#include <stdio.h>
#include <random>
#include <chrono>
#include <unordered_map>
#include <tbb/concurrent_hash_map.h>
#include <xenium/michael_scott_queue.hpp>
#include <string>
#include <cds/init.h>
#include <thread>
#include <mutex>

#include <boost/random.hpp>




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
	
template <typename ValueType, typename Reclaimer, std::size_t Buckets, typename Distribution, typename ...DistributionArgs> 
		static Test MichaelScottQueue(unsigned long operations, 
		unsigned int thread_amount, unsigned long prePopulation, double getProportion, 
		double setProportion, double deleteProportion, DistributionArgs... distribution_args);

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

template <typename ValueType, typename Reclaimer, bool OperationPairs, std::size_t GroupMaxSize, std::size_t Workload, typename Distribution, typename ...DistributionArgs> 
	Test Test::MichaelScottQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, DistributionArgs... distribution_args){

	unsigned long elapsed_time;
	std::thread thread[thread_amount];
	
	xenium::michael_scott_queue
		<ValueType, xenium::policy::reclaimer<Reclaimer>> queue;

	
	{//populacao inicial

		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			queue.push(v);
		}
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){
			
			if constexpr(!OperationPairs){
				if constexpr(GroupMaxSize > 1){
					boost::random::mt19937 chance_generator;
					boost::random::mt19937 amount_generator;
					boost::random::uniform_real_distribution<double> uniform_chance(0.0,1.0);
					boost::random::uniform_int_distribution<uint32_t> uniform_amount(1,20);

					for (size_t i = 0; i < operations/thread_amount; i++){

						uint32_t amount = uniform_amount(amount_generator);
						double chance = uniform(generator);

						if (chance <= 0.5) {
							for (uint32_t i = 0; i < amount; i++){
								ValueType v;
								queue.push(v);
							}
						} else {
							for (uint32_t i = 0; i < amount; i++){
								queue.try_pop();
							}
						}
					}
				} else{
					boost::random::mt19937 generator;
					boost::random::uniform_real_distribution<double> uniform(0.0,1.0);

					for (size_t i = 0; i < operations/thread_amount; i++){

						double chance = uniform(generator);
						if (chance <= 0.5) {
							ValueType v;
							queue.push(v);
						} else {
							queue.try_pop();
						}
					}
				}
			}else {
				if constexpr(GroupMaxSize > 1){
					boost::random::mt19937 generator;
					boost::random::uniform_int_distribution<uint32_t> uniform(1,20);

					for (size_t i = 0; i < operations/thread_amount; i++){

						uint32_t amount = uniform(generator);
						for (uint32_t i = 0; i < amount; i++){
							ValueType v;
							queue.push(v);
						}
						uint32_t amount = uniform(generator);
						for (uint32_t i = 0; i < amount; i++){
							queue.try_pop();
						}
					}
				} else{
					for (size_t i = 0; i < operations/thread_amount; i++){
						ValueType v;
						queue.push(v);
						queue.try_pop();
					}
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

