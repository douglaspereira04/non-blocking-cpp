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
	unsigned long pre_population);

template <typename ValueType> 
	static Test STLQueueSPSC(unsigned long operations, 
	unsigned long pre_population);

template <typename ValueType> 
	static Test BoostQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population);

template <typename ValueType> 
	static Test BoostSPSC(unsigned long operations, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test XeniumMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test XeniumMSSPSC(unsigned long operations, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test CDSMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test CDSMSSPSC(unsigned long operations, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test CDSBasketQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test CDSBasketSPSC(unsigned long operations, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test NikolaevQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population);

template <typename ValueType, typename Reclaimer> 
	static Test NikolaevSPSC(unsigned long operations, 
	unsigned long pre_population);

template <typename ValueType> 
	static Test TBB(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population);

template <typename ValueType> 
	static Test TBBSPSC(unsigned long operations, 
	unsigned long pre_population);

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


std::chrono::microseconds min_loop = std::chrono::microseconds(1);
int loops = 50;

template <typename ValueType, typename Structure, Library L> 
Test spscTest(unsigned long operations, 
unsigned long pre_population){

	unsigned long elapsed_time;
std::chrono::steady_clock::time_point begin;
		
	Structure queue;
	sem_t sem;
	std::mutex mtx;
	sem_init(&sem, 0, 0);
	
	{//populacao inicial
	
		for (uint32_t i = 0; i < pre_population; i++){
			ValueType v;
			if constexpr(L == OTHER || L == LIBCDS || L == STL){
				queue.push(v);
			}
			sem_post(&sem);
		}
	}

	begin = std::chrono::steady_clock::now();

	std::thread producer([&](){
		if constexpr(L == LIBCDS){
			cds::threading::Manager::attachThread();
		}

		for (size_t i = 0; i < operations; i++){

			ValueType v;

			if constexpr(L == OTHER || L == LIBCDS || L == BOOST){
				queue.push(v);
			} else if constexpr(L == STL){
				mtx.lock();
				queue.push(v);
				mtx.unlock();

			}
			
			sem_post(&sem);
			for (size_t i = 0; i < loops; i++){}
			
		}
		
		if constexpr(L == LIBCDS){
			cds::threading::Manager::detachThread();
		}
	});

	std::thread consumer([&](){
		if constexpr(L == LIBCDS){
			cds::threading::Manager::attachThread();
		}
		for (size_t i = 0; i < operations; i++){

			ValueType v;
			sem_wait(&sem);
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
				queue.pop(v);
			} else if constexpr(L == BOOST){
				auto result = queue.try_pull(v);
			}

			for (size_t i = 0; i < loops; i++){}
			

		}

		if constexpr(L == LIBCDS){
			cds::threading::Manager::detachThread();
		}
	});

	producer.join();
	consumer.join();

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	return Test("", elapsed_time, operations, 2);
}

template <typename ValueType, typename Structure, Library L> 
Test testQueue(unsigned long operations, unsigned int thread_amount, 
unsigned long pre_population){

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

			if constexpr(L == LIBCDS){
				cds::threading::Manager::attachThread();
			}
			bool result;

			for (size_t i = 0; i < operations/thread_amount; i++){

				ValueType v;
				if constexpr(L == OTHER){
					queue.push(v);
					for (size_t i = 0; i < loops; i++){}

					result = queue.try_pop(v);
					for (size_t i = 0; i < loops; i++){}
				} else if constexpr(L == STL){
					mtx.lock();
					queue.push(v);
					mtx.unlock();
					for (size_t i = 0; i < loops; i++){}
					
					mtx.lock();
					v = queue.front();
					if(!queue.empty()){
						queue.pop();
					}
					mtx.unlock();
					for (size_t i = 0; i < loops; i++){}

				} else if constexpr(L == LIBCDS){
					queue.push(v);
					for (size_t i = 0; i < loops; i++){}

					result = queue.pop(v);
					for (size_t i = 0; i < loops; i++){}
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

template <typename ValueType, typename Reclaimer> 
	Test Test::NikolaevQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population){
	Test test = testQueue<ValueType, xenium::nikolaev_queue<ValueType, xenium::policy::reclaimer<Reclaimer>,  xenium::policy::entries_per_node<512*32>>, OTHER>(operations, thread_amount, pre_population);
	test.structure = "LSCQ";
	return test;
};
template <typename ValueType, typename Reclaimer> 
	Test Test::NikolaevSPSC(unsigned long operations, 
	unsigned long pre_population){
	Test test = spscTest<ValueType, xenium::nikolaev_queue<ValueType, xenium::policy::reclaimer<Reclaimer>>, OTHER>(operations, pre_population);
	test.structure = "LSCQ";
	return test;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::XeniumMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population){
	Test test = testQueue<ValueType, xenium::michael_scott_queue<ValueType, xenium::policy::reclaimer<Reclaimer>>, OTHER>(operations, thread_amount, pre_population);
	test.structure = "XMSQ";
	return test;
};
template <typename ValueType, typename Reclaimer> 
	Test Test::XeniumMSSPSC(unsigned long operations, 
	unsigned long pre_population){
	Test test = spscTest<ValueType, xenium::michael_scott_queue<ValueType, xenium::policy::reclaimer<Reclaimer>>, OTHER>(operations, pre_population);
	test.structure = "XMSQ";
	return test;
};

		
struct MSTraits: public cds::container::msqueue::traits {
	typedef cds::intrusive::msqueue::stat<> stat;
	typedef cds::atomicity::item_counter item_counter;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::CDSMSQueue(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population){
	typedef cds::container::MSQueue<Reclaimer, ValueType, MSTraits > queue;
	Reclaimer gc;
	cds::Initialize();
	cds::threading::Manager::attachThread();

	Test test = testQueue<ValueType, queue, LIBCDS>(operations, thread_amount, pre_population);

	cds::Terminate();
	test.structure = "CDSMSQ";
	return test;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::CDSMSSPSC(unsigned long operations, 
	unsigned long pre_population){
	typedef cds::container::MSQueue<Reclaimer, ValueType, MSTraits > queue;

	Reclaimer gc;
	cds::Initialize();
	cds::threading::Manager::attachThread();

	Test test = spscTest<ValueType, queue, LIBCDS>(operations, pre_population);

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
	unsigned long pre_population){
	typedef cds::container::BasketQueue<Reclaimer, ValueType, BasketTraits > queue;
	Reclaimer gc;
	cds::Initialize();
	cds::threading::Manager::attachThread();

	Test test = testQueue<ValueType, queue, LIBCDS>(operations, thread_amount, pre_population);

	cds::Terminate();
	test.structure = "BsktQ";
	return test;
};

template <typename ValueType, typename Reclaimer> 
	Test Test::CDSBasketSPSC(unsigned long operations, 
	unsigned long pre_population){
	typedef cds::container::BasketQueue<Reclaimer, ValueType, BasketTraits > queue;

	Reclaimer gc;
	cds::Initialize();
	cds::threading::Manager::attachThread();

	Test test = spscTest<ValueType, queue, LIBCDS>(operations, pre_population);

	cds::Terminate();
	test.structure = "BsktQ";
	return test;
};

template <typename ValueType> 
	Test Test::TBB(unsigned long operations, unsigned int thread_amount, 
	unsigned long pre_population){

	Test test = testQueue<ValueType, tbb::concurrent_queue<ValueType>, OTHER>(operations, thread_amount, pre_population);
	test.structure = "TBBQ";
	return test;
};

template <typename ValueType> 
	Test Test::TBBSPSC(unsigned long operations, 
	unsigned long pre_population){
	Test test = spscTest<ValueType, tbb::concurrent_queue<ValueType>, OTHER>(operations, pre_population);
	test.structure = "TBBQ";
	return test;
};



template <typename ValueType> 
	Test Test::STLQueue(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population){
	Test test = testQueue<ValueType, std::queue<ValueType>, STL>(operations, thread_amount, pre_population);
	test.structure = "STLQ";
	return test;
}

template <typename ValueType> 
	Test Test::STLQueueSPSC(unsigned long operations, 
		unsigned long pre_population){
	Test test = spscTest<ValueType, std::queue<ValueType>, STL>(operations, pre_population);
	test.structure = "STLQ";
	return test;
}

template <typename ValueType> 
	Test Test::BoostQueue(unsigned long operations, unsigned int thread_amount, 
		unsigned long pre_population){
	Test test = testQueue<ValueType, boost::sync_queue<ValueType>, BOOST>(operations,thread_amount, pre_population);
	test.structure = "BoostQ";
	return test;
}

template <typename ValueType> 
	Test Test::BoostSPSC(unsigned long operations, 
		unsigned long pre_population){
	Test test = spscTest<ValueType, boost::sync_queue<ValueType>, BOOST>(operations, pre_population);
	test.structure = "BoostQ";
	return test;
}