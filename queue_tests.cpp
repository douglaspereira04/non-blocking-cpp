#include <stdio.h>
#include <random>
#include <chrono>
#include <unordered_map>
#include <tbb/concurrent_queue.h>
#include <xenium/michael_scott_queue.hpp>
#include <xenium/nikolaev_queue.hpp>
#include <string>
#include <cds/init.h>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <queue>

#include <boost/thread/sync_queue.hpp>

#include <boost/random.hpp>

#include <cds/container/msqueue.h>
#include <cds/container/basket_queue.h>
#include <cds/init.h>


enum Library{
	LIBCDS, STL, OTHER, BOOST
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
	
template <typename ValueType> 
	static Test STLQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

template <typename ValueType> 
	static Test BoostQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

template <typename ValueType, typename Reclaimer> 
	static Test XeniumMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

template <typename ValueType, typename Reclaimer> 
	static Test CDSMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

template <typename ValueType, typename Reclaimer> 
	static Test CDSBasketQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

template <typename ValueType, typename Reclaimer> 
	static Test NikolaevQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

template <typename ValueType> 
	static Test TBB(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion);

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


template <typename ValueType, typename Structure, Library L> 
Test testQueue(unsigned long operations, unsigned int thread_amount, 
unsigned long pre_population, double insert_proportion){

	unsigned long elapsed_time;
	std::chrono::steady_clock::time_point begin;

	std::thread thread[thread_amount];

	Structure queue;
	std::mutex mtx;

	{//populacao inicial
	
		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			if constexpr(L == OTHER || L == LIBCDS || L == STL){
				queue.push(v);
			}
		}
	}

	begin = std::chrono::steady_clock::now();

	for (size_t i = 0; i < thread_amount; i++){
		thread[i] = std::thread([&](){

			boost::random::mt19937 operation_generator;
			boost::random::mt19937 work_generator;
			boost::random::uniform_real_distribution<double> operation_rand(0.0,1.0);
			boost::random::uniform_int_distribution<int> work_rand(0, 1000);

			if constexpr(L == LIBCDS){
				cds::threading::Manager::attachThread();
			}

			for (size_t i = 0; i < operations/thread_amount; i++){
				double chance = operation_rand(operation_generator);

				ValueType v;
				if (chance <= insert_proportion) {

					if constexpr(L == OTHER || L == LIBCDS || L == BOOST){
						queue.push(v);
					} else if constexpr(L == STL){
						mtx.lock();
						queue.push(v);
						mtx.unlock();
					}

				} else {
					if constexpr(L == OTHER){
						bool result = queue.try_pop(v);
					} else if constexpr(L == STL){

						mtx.lock();
						v = queue.front();
						if(!queue.empty()){
							queue.pop();
						}
						mtx.unlock();
					} else if constexpr(L == LIBCDS){
						bool result = queue.pop(v);
					} else if constexpr(L == BOOST){
						auto result = queue.try_pull(v);
					}
				}
				int loops = work_rand(work_generator);
				for (size_t i = 0; i < loops; i++){}
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

template <typename ValueType, typename Reclaimer> 
	Test Test::NikolaevQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion){
	Test test = testQueue<ValueType, xenium::nikolaev_queue<ValueType, xenium::policy::reclaimer<Reclaimer>,  xenium::policy::entries_per_node<512*32>>, OTHER>(operations, thread_amount, pre_population, insert_proportion);
	test.structure = "LSCQ";
	return test;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::XeniumMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion){
	Test test = testQueue<ValueType, xenium::michael_scott_queue<ValueType, xenium::policy::reclaimer<Reclaimer>>, OTHER>(operations, thread_amount, pre_population, insert_proportion);
	test.structure = "XMSQ";
	return test;
};

		
struct MSTraits: public cds::container::msqueue::traits {
	typedef cds::intrusive::msqueue::stat<> stat;
	typedef cds::atomicity::item_counter item_counter;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::CDSMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion){
	typedef cds::container::MSQueue<Reclaimer, ValueType, MSTraits > queue;
	Reclaimer gc;
	cds::Initialize();
	cds::threading::Manager::attachThread();

	Test test = testQueue<ValueType, queue, LIBCDS>(operations, thread_amount, pre_population, insert_proportion);

	cds::Terminate();
	test.structure = "CDSMSQ";
	return test;
};


struct BasketTraits: public cds::container::basket_queue::traits {
	typedef cds::intrusive::basket_queue::stat<> stat;
	typedef cds::atomicity::item_counter item_counter;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::CDSBasketQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion){
	typedef cds::container::BasketQueue<Reclaimer, ValueType, BasketTraits > queue;
	Reclaimer gc;
	cds::Initialize();
	cds::threading::Manager::attachThread();

	Test test = testQueue<ValueType, queue, LIBCDS>(operations, thread_amount, pre_population, insert_proportion);

	cds::Terminate();
	test.structure = "Bskt";
	return test;
};

template <typename ValueType> 
	Test Test::TBB(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population, double insert_proportion){

	Test test = testQueue<ValueType, tbb::concurrent_queue<ValueType>, OTHER>(operations, thread_amount, pre_population, insert_proportion);
	test.structure = "TBB";
	return test;
};


template <typename ValueType> 
	Test Test::STLQueue(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double insert_proportion){
	Test test = testQueue<ValueType, std::queue<ValueType>, STL>(operations, thread_amount, pre_population, insert_proportion);
	test.structure = "STL";
	return test;
}

template <typename ValueType> 
	Test Test::BoostQueue(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population, double insert_proportion){
	Test test = testQueue<ValueType, boost::sync_queue<ValueType>, BOOST>(operations, thread_amount, pre_population, insert_proportion);
	test.structure = "Boost";
	return test;
}
