#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/policy.hpp>
#include <xenium/reclamation/lock_free_ref_count.hpp>


class Test{
protected:

	unsigned long elapsedTime;
	unsigned long operations;
	unsigned int n_threads;

public:

	Test(unsigned long elapsedTime, unsigned long operations, unsigned int n_threads);
	~Test();
	
	static Test HarrisMichaelMapTest(unsigned long operations, unsigned int n_threads, unsigned int buckets);
};


Test::Test(unsigned long elapsedTime, unsigned long operations, unsigned int n_threads){
	Test::elapsedTime = elapsedTime;
	Test::operations = operations;
	Test::n_threads = n_threads;
};

Test::~Test(){};

static Test HarrisMichaelMapRefCount10Test(unsigned long operations, unsigned int n_threads, unsigned int buckets, unsigned long prePopulation, double getProportion, double setProportion, double deleteProportion){
	xenium::harris_michael_hash_map<std::string, int, xenium::policy::reclaimer<xenium::reclamation::lock_free_ref_count<>>, xenium::policy::buckets<10>> map;

	

	Test test_result(1U, 1U, 1U);
	return test_result;	
};