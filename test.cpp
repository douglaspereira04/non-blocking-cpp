#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/policy.hpp>
#include <xenium/reclamation/lock_free_ref_count.hpp>
#include <omp.h>
#include <stdio.h>
#include <random>
#include <chrono>


class Test{
protected:

	unsigned long elapsedTime;
	unsigned long operations;
	unsigned int n_threads;

public:

	Test(unsigned long elapsedTime, unsigned long operations, unsigned int n_threads);
	~Test();
	
	template <typename Distribution> static Test HarrisMichaelMapRefCount16Test(unsigned long operations, 
		unsigned int n_threads, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, Distribution distribution);
};


Test::Test(unsigned long elapsedTime, unsigned long operations, unsigned int n_threads){
	Test::elapsedTime = elapsedTime;
	Test::operations = operations;
	Test::n_threads = n_threads;
};

Test::~Test(){};

template <typename Distribution> Test Test::HarrisMichaelMapRefCount16Test(unsigned long operations, 
	unsigned int n_threads, unsigned long prePopulation, double getProportion, 
	double setProportion, double deleteProportion, Distribution distribution){
	xenium::harris_michael_hash_map<int, int, xenium::policy::reclaimer<xenium::reclamation::lock_free_ref_count<>>, xenium::policy::buckets<2048>> map;
	
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

	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	Test test_result = Test(1U, 1U, 1U);
	return test_result;	
};