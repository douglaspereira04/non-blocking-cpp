#include <omp.h>
#include <stdio.h>
#include <random>
#include <chrono>
#include <unordered_map>
//#include <tbb/concurrent_hash_map.h>
#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/vyukov_hash_map.hpp>
#include <string>
#include <wfc/unordered_map.hpp>
#include <cds/container/feldman_hashmap_dhp.h>


class Test{
protected:

	std::string structure;
	unsigned long elapsed_time;
	unsigned long operations;
	unsigned int thread_amount;
	double throughput;

public:

	Test(std::string structure, unsigned long elapsed_time, unsigned long operations, unsigned int thread_amount);
	~Test();
	
	template <typename Distribution, typename Reclaimer, std::size_t Buckets> 
		static Test HarrisMichaelMapTest(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);

	template <typename Distribution> 
		static Test LockUnorderedMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);
/*
	template <typename Distribution> 
		static Test TBBMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);*/

	template <typename Distribution, typename Reclaimer> 
		static Test Vyukov(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);

	template <typename Distribution> 
		static Test WFCLabordeWaitFree(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution, std::size_t node_array_size);

	template <typename Distribution, typename GarbageCollector> 
		static Test LibCDSFeldman(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);

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
	unsigned long Throughput(){
		return this->throughput;
	};
};


Test::Test(std::string structure, unsigned long elapsed_time, unsigned long operations, unsigned int thread_amount){
	Test::structure = structure;
	Test::elapsed_time = elapsed_time;
	Test::operations = operations;
	Test::thread_amount = thread_amount;
	Test::throughput = ((double)operations/elapsed_time)*10e9;
};

Test::~Test(){};

template <typename Distribution, typename Reclaimer, std::size_t Buckets> 
	Test Test::HarrisMichaelMapTest(unsigned long operations, 
	unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
	double setProportion, double deleteProportion, Distribution distribution){
	xenium::harris_michael_hash_map
		<int, int, xenium::policy::reclaimer
			<Reclaimer>, xenium::policy::buckets<Buckets>> map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(thread_amount);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for schedule(static,1) private(generator, uniform)
	for (size_t i = 0; i < operations; i++){
		double chance = uniform(generator);
		int key = (int)distribution(generator);

		if (chance <= getProportion) {
			// get
			map.find(key);
		} else if (chance <= (getProportion + setProportion)) {
			// set
			map.emplace(key, key);
		} else {
			// delete
			map.erase(key);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("XeniumHarrisMichael", elapsed_time, operations, thread_amount);
};
template <typename Distribution> 
		Test Test::LockUnorderedMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution){
	std::unordered_map<int, int> map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(thread_amount);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for schedule(static,1) private(generator, uniform)
	for (size_t i = 0; i < operations; i++){
		double chance = uniform(generator);
		int key = (int)distribution(generator);

		if (chance <= getProportion) {
			// get
			#pragma omp critical
				map.find(key);
		} else if (chance <= (getProportion + setProportion)) {
			// set
			#pragma omp critical
				map.emplace(key, key);
		} else {
			// delete
			#pragma omp critical
				map.erase(key);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("LockUnordered", elapsed_time, operations, thread_amount);
};
/*
template <typename Distribution> 
		Test Test::TBBMap(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution){
	typedef tbb::concurrent_hash_map<int,int> tbb_map;
	tbb_map map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(thread_amount);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for schedule(static,1) private(generator, uniform)
	for (size_t i = 0; i < operations; i++){
		double chance = uniform(generator);
		int key = (int)distribution(generator);

		if (chance <= getProportion) {
			// get
			tbb_map::accessor a;
			map.find(a, key);
		} else if (chance <= (getProportion + setProportion)) {
			// set
			tbb_map::accessor a;
			map.emplace(a, key, key);
		} else {
			// delete
			map.erase(key);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("TBB", elapsed_time, operations, thread_amount);
};
*/

template <typename Distribution> 
		Test Test::WFCLabordeWaitFree(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution, std::size_t node_array_size){
	
	typedef wfc::unordered_map<int,int> laborde_map;
	laborde_map map(node_array_size, thread_amount, thread_amount);
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(thread_amount);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for schedule(static,1) private(generator, uniform)
	for (size_t i = 0; i < operations; i++){
		double chance = uniform(generator);
		int key = (int)distribution(generator);

		if (chance <= getProportion) {
			// get
			map.get(key);
		} else if (chance <= (getProportion + setProportion)) {
			// set
			map.insert(key, key);
		} else {
			// delete
			map.remove(key);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("LabordeWaitFree", elapsed_time, operations, thread_amount);
};

template <typename Distribution, typename GarbageCollector> 
		Test Test::LibCDSFeldman(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution){
	
    typedef cds::container::FeldmanHashMap<
		GarbageCollector, int, int, 
		cds::container::feldman_hashmap::traits> feldman_map;
    feldman_map map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(thread_amount);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for schedule(static,1) private(generator, uniform)
	for (size_t i = 0; i < operations; i++){
		double chance = uniform(generator);
		int key = (int)distribution(generator);

		if (chance <= getProportion) {
			// get
			map.get(key);
		} else if (chance <= (getProportion + setProportion)) {
			// set
			map.insert(key, key);
		} else {
			// delete
			map.erase(key);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	unsigned long elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("LabordeWaitFree", elapsed_time, operations, thread_amount);
};