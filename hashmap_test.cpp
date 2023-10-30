#include <stdio.h>
#include <random>
#include <chrono>
#include <unordered_map>
#include <tbb/concurrent_hash_map.h>
#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/utils.hpp>
#include <string>
#include <wfc/unordered_map.hpp>
#include <cds/container/michael_list_hp.h>
#include <cds/container/feldman_hashmap_hp.h>
#include <cds/container/feldman_hashmap_dhp.h>
#include <cds/container/michael_kvlist_hp.h>
#include <cds/container/michael_kvlist_dhp.h>
#include <cds/container/michael_map.h>
#include <cds/container/split_list_map.h>
#include <cds/init.h>
#include <thread>
#include <mutex>

#include <boost/random.hpp>



enum LIB{
	LIBCDS, STL, TBB, XENIUM, WFC
};

class Test{
protected:

	std::string structure;
	unsigned long elapsed_time;
	unsigned long operations;
	unsigned int thread_amount;
	double throughput;

public:
	Test();
	Test(std::string structure, unsigned long elapsed_time, unsigned long operations, unsigned int thread_amount);
	~Test();
	
template <typename value_t, typename reclaimer_t, std::size_t Buckets, typename dist_t, typename ...dist_args_t> 
		static Test XeniumMichael(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, dist_args_t... distribution_args);

	template <typename value_t, typename dist_t, typename ...dist_args_t> 
		static Test STDLock(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, dist_args_t... distribution_args);
		
	template <typename value_t, typename dist_t, typename ...dist_args_t> 
		static Test TBBMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, dist_args_t... distribution_args);

	template <typename value_t, std::size_t node_array_size, typename dist_t, typename ...dist_args_t> 
		static Test WFCUnorderedMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, dist_args_t... distribution_args);

	template <typename value_t, typename gc_t, size_t head_bits, size_t array_bits, typename dist_t, typename ...dist_args_t> 
		static Test LibCDSFeldman(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double get_proportion, double set_proportion, 
		double delete_proportion, dist_args_t... distribution_args);

	template <typename value_t, typename gc_t, size_t MaxItemCount, size_t LoadFactor, typename dist_t, typename ...dist_args_t> 
		static Test LibCDSMichael(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double get_proportion, double set_proportion, 
		double delete_proportion, dist_args_t... distribution_args);

	template <typename value_t, typename gc_t, size_t MaxItemCount, size_t LoadFactor, typename dist_t, typename ...dist_args_t> 
		static Test LibCDSSplitOrdered(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double get_proportion, double set_proportion, 
		double delete_proportion, dist_args_t... distribution_args);


	std::string Structure(){
		return this->structure;
	};
	unsigned long ElapsedTime(){
		return this->elapsed_time;
	};
	unsigned long Operations(){
		return this->operations;
	};
	unsigned long ThreadAmount(){
		return this->thread_amount;
	};
	double Throughput(){
		return this->throughput;
	};
};

Test::Test(){

};
Test::Test(std::string structure, unsigned long elapsed_time, unsigned long operations, unsigned int thread_amount){
	Test::structure = structure;
	Test::elapsed_time = elapsed_time;
	Test::operations = operations;
	Test::thread_amount = thread_amount;
	Test::throughput = ((double)operations/elapsed_time)*10e9;
};

Test::~Test(){};

template <typename value_t, typename map_t, LIB L, typename dist_t, typename ...dist_args_t> 
Test table_test(map_t *map, unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){

	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	std::mutex mtx;


	
	{//populacao inicial
		for (uint32_t i = 0; i < pre_population; i++){
			value_t v;
			if constexpr(L == XENIUM || L == STL){
				map->emplace(i, v);
			} else if constexpr(L == TBB){
				typename map_t::accessor a;
				map->emplace(a, i, v);
			} else if constexpr(L == WFC || L == LIBCDS){
				map->insert(i,v);
			}
		}
	}


	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			if constexpr(L == LIBCDS){
				cds::threading::Manager::attachThread();
			}

			boost::random::mt19937 generator;
			dist_t distribution(distribution_args...);
			boost::random::uniform_real_distribution<double> uniform(0.0,1.0);
			
			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = uniform(generator);
				uint32_t key = (uint32_t)distribution(generator);
				
				if (chance <= get_proportion) {
					// get
					
					if constexpr(L == STL){
						mtx.lock();
						map->find(key);
						mtx.unlock();
					} else if constexpr(L == XENIUM ){
						(*map)[key];
					} else if constexpr(L == TBB){
						typename map_t::accessor a;
						map->find(a, key);
					} else if constexpr(L == WFC){
						map->get(key);
					} else if constexpr(L == LIBCDS){
						typedef typename map_t::guarded_ptr gp_t;
						gp_t gp = gp_t( map->get(key));
					}
				} else if (chance <= (get_proportion + set_proportion)) {
					// set
					value_t v;
					if constexpr(L == STL){
						mtx.lock();
						map->emplace(key, v);
						mtx.unlock();
					} else if constexpr(L == XENIUM ){
						map->emplace(key, v);
					} else if constexpr(L == TBB){
						typename map_t::accessor a;
						map->emplace(a, key, v);
					} else if constexpr(L == WFC || L == LIBCDS){
						map->insert(key,v);
					}
				} else {
					// delete
					if constexpr(L == STL){
						mtx.lock();
						map->erase(key);
						mtx.unlock();
					} else if constexpr(L == XENIUM || L == TBB || LIBCDS){
						map->erase(key);
					} else if constexpr(L == WFC ){
						map->remove(key);
					}
				}
			}

			if constexpr(L == LIBCDS){
				cds::threading::Manager::detachThread();
			}
		});
	}

	for (size_t i = 0; i < thread_amount; i++){
		thread[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

	return Test("", elapsed_time, operations, thread_amount);
}

template <typename value_t, typename dist_t, typename ...dist_args_t> 
Test Test::STDLock(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){
	
	typedef std::unordered_map<uint32_t, value_t> map_t;
	map_t *map = new map_t();
	Test result = table_test<value_t, map_t, STL, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
	delete map;
	result.structure = "STD_Lock";

	return result;
};

template <typename value_t, typename reclaimer_t, std::size_t Buckets, typename dist_t, typename ...dist_args_t> 
	Test Test::XeniumMichael(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){

	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	
	typedef xenium::harris_michael_hash_map
		<uint32_t, value_t, xenium::policy::reclaimer
			<reclaimer_t>, xenium::policy::buckets<Buckets>,
			xenium::policy::map_to_bucket<xenium::utils::modulo<uint32_t>>> map_t;

	map_t *map = new map_t();

	Test result = table_test<value_t, map_t, XENIUM, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
	delete map;
	result.structure = "Xenium_Michael";

	return result;
};

template <typename value_t, typename dist_t, typename ...dist_args_t> 
Test Test::TBBMap(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){


	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	typedef tbb::concurrent_hash_map<uint32_t,value_t> map_t;
	map_t *map = new map_t();
	
	Test result = table_test<value_t, map_t, TBB, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
	delete map;
	result.structure = "TBB";

	return result;
};


template <typename value_t, std::size_t node_array_size, typename dist_t, typename ...dist_args_t> 
Test Test::WFCUnorderedMap(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){
	
	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	typedef wfc::unordered_map<uint32_t,value_t> map_t;
	map_t *map = new map_t(node_array_size, thread_amount, thread_amount);
	
	Test result = table_test<value_t, map_t, WFC, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
	delete map;
	result.structure = "WFC_Laborde";

	return result;
};


template <typename value_t, typename gc_t, size_t head_bits, size_t array_bits, typename dist_t, typename ...dist_args_t> 
Test Test::LibCDSFeldman(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){
	
	unsigned long elapsed_time;
	
	std::thread thread[thread_amount];
	Test result;
    cds::Initialize();
    {
        gc_t gc;
        cds::threading::Manager::attachThread();

        typedef cds::container::FeldmanHashMap<gc_t, uint32_t, value_t> map_t;
		map_t *map = new map_t(head_bits,array_bits);

		result = table_test<value_t, map_t, LIBCDS, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
		delete map;

		
    }    
    cds::Terminate();
	result.structure = "LibCDS_Feldman";
	return result;

};

template <typename value_t, typename gc_t, size_t MaxItemCount, size_t LoadFactor, typename dist_t, typename ...dist_args_t> 
Test Test::LibCDSMichael(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){
	
	unsigned long elapsed_time;
	
	std::thread thread[thread_amount];
	Test result;
    cds::Initialize();
    {
        gc_t gc;
        cds::threading::Manager::attachThread();
	
		typedef cds::container::MichaelHashMap<gc_t, cds::container::MichaelKVList<gc_t, uint32_t, value_t>> map_t;
		map_t *map = new map_t(MaxItemCount, LoadFactor);
		result = table_test<value_t, map_t, LIBCDS, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
		delete map;

		
    }    
    cds::Terminate();
	result.structure = "LibCDS_Michael";
	return result;

};

template <typename value_t, typename gc_t, size_t MaxItemCount, size_t LoadFactor, typename dist_t, typename ...dist_args_t> 
Test Test::LibCDSSplitOrdered(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, dist_args_t... distribution_args){
	
	unsigned long elapsed_time;
	
	std::thread thread[thread_amount];
	Test result;
    cds::Initialize();
    {
        gc_t gc;
        cds::threading::Manager::attachThread();
		
		typedef cds::container::SplitListMap<
			gc_t,
			uint32_t,
			value_t,
			cds::container::split_list::make_traits<      // metafunction to build split-list traits
				cds::container::split_list::ordered_list<cds::container::michael_list_tag>,
				cds::container::opt::hash< std::hash<uint32_t> >,
				cds::container::split_list::ordered_list_traits<    // ordered list traits desired
					cds::container::michael_list::make_traits<    // metafunction to build lazy list traits
						cds::container::opt::less< std::less<uint32_t> >         // less-based compare functor
					>::type
				>
			>::type
		>  map_t;
		map_t *map = new map_t(MaxItemCount, LoadFactor);
		result = table_test<value_t, map_t, LIBCDS, dist_t, dist_args_t...>(map, operations, thread_amount, pre_population, get_proportion, set_proportion, delete_proportion, distribution_args...);
		delete map;

		
    }    
    cds::Terminate();
	result.structure = "LibCDS_SplitOrdered";
	return result;

};