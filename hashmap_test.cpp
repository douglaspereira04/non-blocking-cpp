#include <stdio.h>
#include <random>
#include <chrono>
#include <unordered_map>
#include <tbb/concurrent_hash_map.h>
#include <xenium/harris_michael_hash_map.hpp>
#include <string>
#include <wfc/unordered_map.hpp>

#include <cds/container/feldman_hashmap_hp.h>
#include <cds/container/feldman_hashmap_dhp.h>
#include <cds/container/michael_kvlist_hp.h>
#include <cds/container/michael_kvlist_dhp.h>
#include <cds/container/michael_map.h>
#include <lockfreehashtable/lockfree_hashtable.h>
#include <cds/init.h>
#include <thread>
#include <mutex>

#include <boost/random.hpp>




class Test{
protected:
#include <lockfreehashtable/lockfree_hashtable.h>

	std::string structure;
	unsigned long elapsed_time;
	unsigned long operations;
	unsigned int thread_amount;
	double throughput;

public:
	Test();
	Test(std::string structure, unsigned long elapsed_time, unsigned long operations, unsigned int thread_amount);
	~Test();
	
template <typename ValueType, typename Reclaimer, std::size_t Buckets, typename Distribution, typename ...DistributionArgs> 
		static Test XeniumHarrisMichael(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, DistributionArgs... distribution_args);

	template <typename ValueType, typename Distribution, typename ...DistributionArgs> 
		static Test LockUnordered(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, DistributionArgs... distribution_args);
		
	template <typename ValueType, typename Distribution, typename ...DistributionArgs> 
		static Test TBBMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, DistributionArgs... distribution_args);

	template <typename ValueType, std::size_t node_array_size, typename Distribution, typename ...DistributionArgs> 
		static Test WFCUnorderedMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, DistributionArgs... distribution_args);

	template <typename ValueType, typename Distribution, typename ...DistributionArgs> 
		static Test Bhhbazinga(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, DistributionArgs... distribution_args);

	template <typename ValueType, typename GC, size_t head_bits, size_t array_bits, typename Distribution, typename ...DistributionArgs> 
		static Test LibCDSFeldman(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double get_proportion, double set_proportion, 
		double delete_proportion, DistributionArgs... distribution_args);

	template <typename ValueType, typename GC, size_t MaxItemCount, size_t LoadFactor, typename Distribution, typename ...DistributionArgs> 
		static Test LibCDSMichael(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double get_proportion, double set_proportion, 
		double delete_proportion, DistributionArgs... distribution_args);


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

template <typename ValueType, typename Reclaimer, std::size_t Buckets, typename Distribution, typename ...DistributionArgs> 
	Test Test::XeniumHarrisMichael(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){

	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	
	xenium::harris_michael_hash_map
		<uint32_t, ValueType, xenium::policy::reclaimer
			<Reclaimer>, xenium::policy::buckets<Buckets>> map;

	
	{//populacao inicial
		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			map.emplace(i, v);
		}
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			boost::random::mt19937 generator;
			Distribution distribution(distribution_args...);
			boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = uniform(generator);
				uint32_t key = (uint32_t)distribution(generator);

				if (chance <= get_proportion) {
					// get
					map.find(key);
				} else if (chance <= (get_proportion + set_proportion)) {
					// set
					ValueType v;
					map.emplace(key, v);
				} else {
					// delete
					map.erase(key);
				}
			}
		});
	}

	for (size_t i = 0; i < thread_amount; i++){
		thread[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("XeniumHarrisMichael", elapsed_time, operations, thread_amount);
};


template <typename ValueType, typename Distribution, typename ...DistributionArgs> 
Test Test::LockUnordered(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){

	std::mutex mutex;
	
	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	std::unordered_map<uint32_t, ValueType> map;
	
	{//populacao inicial
		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			map.emplace(i, v);
		}
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			boost::random::mt19937 generator;
			Distribution distribution(distribution_args...);
			boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = uniform(generator);
				uint32_t key = (uint32_t)distribution(generator);

				if (chance <= get_proportion) {
					// get
					mutex.lock();
					map.find(key);
					mutex.unlock();
				} else if (chance <= (get_proportion + set_proportion)) {
					// set
					ValueType v;
					mutex.lock();
					map.emplace(key, v);
					mutex.unlock();
				} else {
					// delete
					mutex.lock();
					map.erase(key);
					mutex.unlock();
				}
			}
		});
	}

	for (size_t i = 0; i < thread_amount; i++){
		thread[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("LockUnordered", elapsed_time, operations, thread_amount);
};

template <typename ValueType, typename Distribution, typename ...DistributionArgs> 
Test Test::TBBMap(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){


	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	typedef tbb::concurrent_hash_map<uint32_t,ValueType> tbb_map;
	tbb_map map;
	

	{//populacao inicial

		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			typename tbb_map::accessor a;
			map.emplace(a, i, v);
		}
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			boost::random::mt19937 generator;
			Distribution distribution(distribution_args...);
			boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = uniform(generator);
				uint32_t key = (uint32_t)distribution(generator);

				if (chance <= get_proportion) {
					// get
					typename tbb_map::accessor a;
					map.find(a, key);
				} else if (chance <= (get_proportion + set_proportion)) {
					// set
					ValueType v;
					typename tbb_map::accessor a;
					map.emplace(a, key, v);
				} else {
					// delete
					map.erase(key);
				}
			}
		});
	}

	for (size_t i = 0; i < thread_amount; i++){
		thread[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("TBB", elapsed_time, operations, thread_amount);
};


template <typename ValueType, std::size_t node_array_size, typename Distribution, typename ...DistributionArgs> 
Test Test::WFCUnorderedMap(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){
	
	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	wfc::unordered_map<uint32_t,ValueType> map(node_array_size, thread_amount, thread_amount);

	{//populacao inicial
		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			map.insert(i,v);
		}
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			boost::random::mt19937 generator;
			Distribution distribution(distribution_args...);
			boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = uniform(generator);
				uint32_t key = (uint32_t)distribution(generator);

				if (chance <= get_proportion) {
					// get
					map.get(key);
				} else if (chance <= (get_proportion + set_proportion)) {
					// set
					ValueType v;
					map.insert(key, v);
				} else {
					// delete
					map.remove(key);
				}
			}
		});
	}

	for (size_t i = 0; i < thread_amount; i++){
		thread[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("WFCUnorderedMap", elapsed_time, operations, thread_amount);
};


template <typename ValueType, typename Distribution, typename ...DistributionArgs> 
Test Test::Bhhbazinga(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){
	
	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	LockFreeHashTable<uint32_t,ValueType> map;

	{//populacao inicial
		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			map.Insert(i,v);
		}
	}
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			boost::random::mt19937 generator;
			Distribution distribution(distribution_args...);
			boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = uniform(generator);
				uint32_t key = (uint32_t)distribution(generator);

				if (chance <= get_proportion) {
					// get
					ValueType v;
					map.Find(key, v);
				} else if (chance <= (get_proportion + set_proportion)) {
					// set
					ValueType v;
					map.Insert(key, v);
				} else {
					// delete
					map.Delete(key);
				}
			}
		});
	}

	for (size_t i = 0; i < thread_amount; i++){
		thread[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("Bhhbazinga", elapsed_time, operations, thread_amount);
};

template <typename ValueType, typename GC, size_t head_bits, size_t array_bits, typename Distribution, typename ...DistributionArgs> 
Test Test::LibCDSFeldman(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){
	
	unsigned long elapsed_time;
	
	std::thread thread[thread_amount];
    cds::Initialize();
    {
        GC gc;
        cds::threading::Manager::attachThread();
        cds::container::FeldmanHashMap<GC, uint32_t, ValueType> map(head_bits,array_bits);
		typedef typename cds::container::FeldmanHashMap<GC, uint32_t, ValueType>::guarded_ptr GuardedPointer;

		{//populacao inicial
			for (uint32_t i = 0; i < pre_population; i++){
				ValueType v;
				map.insert(i,v);
			}
		}
		
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		
		for (size_t i = 0; i < thread_amount; i++){
			thread[i] = std::thread([&](){
		
				boost::random::mt19937 generator;
				Distribution distribution(distribution_args...);
				boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

				cds::threading::Manager::attachThread();

				for (size_t i = 0; i < operations/thread_amount; i++){
					double chance = uniform(generator);
					uint32_t key = (uint32_t)distribution(generator);

					if (chance <= get_proportion) {
						// get
						GuardedPointer gp = GuardedPointer( map.get(key));
					} else if (chance <= (get_proportion + set_proportion)) {
						// set
						ValueType v;
						map.insert(key,v);
					} else {
						// delete
						map.erase(key);
					}
				}

				cds::threading::Manager::detachThread();
			});
		}

		for (size_t i = 0; i < thread_amount; i++){
			thread[i].join();
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		
    }    
    cds::Terminate();
	
	return Test("LibCDSFeldman", elapsed_time, operations, thread_amount);

};

template <typename ValueType, typename GC, size_t MaxItemCount, size_t LoadFactor, typename Distribution, typename ...DistributionArgs> 
Test Test::LibCDSMichael(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double get_proportion, double set_proportion, 
	double delete_proportion, DistributionArgs... distribution_args){
	
	unsigned long elapsed_time;
	
	std::thread thread[thread_amount];
    cds::Initialize();
    {
        GC gc;
        cds::threading::Manager::attachThread();
	
		cds::container::MichaelHashMap<GC, cds::container::MichaelKVList<GC, uint32_t, ValueType>> map(MaxItemCount, LoadFactor);
		typedef typename cds::container::MichaelHashMap<GC, cds::container::MichaelKVList<GC, uint32_t, ValueType>>::guarded_ptr GuardedPointer;

		{//populacao inicial
			for (uint32_t i = 0; i < pre_population; i++){
				ValueType v;
				map.insert(i,v);
			}
		}
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (size_t i = 0; i < thread_amount; i++){
			thread[i] = std::thread([&](){
		
				boost::random::mt19937 generator;
				Distribution distribution(distribution_args...);
				boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

				cds::threading::Manager::attachThread();

				for (size_t i = 0; i < operations/thread_amount; i++){
					double chance = uniform(generator);
					uint32_t key = (uint32_t)distribution(generator);

					if (chance <= get_proportion) {
						// get
						GuardedPointer gp = GuardedPointer( map.get(key));
					} else if (chance <= (get_proportion + set_proportion)) {
						// set
						ValueType v;
						map.insert(key,v);
					} else {
						// delete
						map.erase(key);
					}
				}

				cds::threading::Manager::detachThread();
			});
		}

		for (size_t i = 0; i < thread_amount; i++){
			thread[i].join();
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
		
    }    
    cds::Terminate();
	
	return Test("LibCDSMichael", elapsed_time, operations, thread_amount);

};
