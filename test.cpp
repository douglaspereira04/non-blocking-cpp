#include <omp.h>
#include <stdio.h>
#include <random>
#include <chrono>
#include <unordered_map>
#include <tbb/concurrent_hash_map.h>
#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/vyukov_hash_map.hpp>


class Test{
protected:

	unsigned long elapsedTime;
	unsigned long operations;
	unsigned int n_threads;

public:

	Test(unsigned long elapsedTime, unsigned long operations, unsigned int n_threads);
	~Test();
	
	template <typename Distribution, typename Reclaimer, std::size_t Buckets> 
		static Test HarrisMichaelMapTest(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);

	template <typename Distribution> 
		static Test LockUnorderedMap(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);

	template <typename Distribution> 
		static Test TBBMap(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);

	template <typename Distribution, typename Reclaimer> 
		static Test Vyukov(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);
};


Test::Test(unsigned long elapsedTime, unsigned long operations, unsigned int n_threads){
	Test::elapsedTime = elapsedTime;
	Test::operations = operations;
	Test::n_threads = n_threads;
};

Test::~Test(){};

template <typename Distribution, typename Reclaimer, std::size_t Buckets> 
	Test Test::HarrisMichaelMapTest(unsigned long operations, 
	unsigned int n_threads, unsigned long prePopulation, double getProportion, 
	double setProportion, double deleteProportion, Distribution distribution){
	xenium::harris_michael_hash_map
		<int, int, xenium::policy::reclaimer
			<Reclaimer>, xenium::policy::buckets<Buckets>> map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(n_threads);

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

	std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	Test test_result = Test(1U, 1U, 1U);
	return test_result;	
};
template <typename Distribution> 
		Test Test::LockUnorderedMap(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution){
	std::unordered_map<int, int> map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(n_threads);

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

	std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	Test test_result = Test(1U, 1U, 1U);
	return test_result;	
};

template <typename Distribution> 
		Test Test::TBBMap(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution){
	typedef tbb::concurrent_hash_map<int,int> tbb_map;
	tbb_map map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(n_threads);

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

	std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	Test test_result = Test(1U, 1U, 1U);
	return test_result;	
};

template <typename Distribution, typename Reclaimer> 
		Test Test::Vyukov(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution){
	typedef xenium::vyukov_hash_map<int,int, xenium::policy::reclaimer<Reclaimer>> vyukov_map;
	vyukov_map map;
	
  	std::default_random_engine generator;
	std::uniform_real_distribution<double> uniform(0.0,1.0);
	
	omp_set_num_threads(n_threads);

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

	std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	Test test_result = Test(1U, 1U, 1U);
	return test_result;	
};