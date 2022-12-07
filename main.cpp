#include <iostream>
#include <random>
#include "test.cpp"
#include <xenium/harris_michael_hash_map.hpp>
#include <xenium/policy.hpp>
#include <xenium/reclamation/lock_free_ref_count.hpp>
#include <xenium/reclamation/hazard_pointer.hpp>
#include <xenium/reclamation/hazard_eras.hpp>
#include <xenium/reclamation/stamp_it.hpp>
#include <xenium/reclamation/quiescent_state_based.hpp>
#include <fstream>
#include <string>
#include <cds/gc/dhp.h>
#include <cds/gc/hp.h>
#include <cds/urcu/general_buffered.h>

std::ofstream TEST_FILE;
std::string TEST_FILE_NAME = "test.csv";

void clear_file(){
    TEST_FILE.open (TEST_FILE_NAME);
    TEST_FILE << "Structure,Elapsed Time,Operations,Threads,Throughput,BucketAmount,Reclamation\n";
    TEST_FILE.close();
}
void append_to_file(Test test, std::size_t buckets, std::string reclamation){

    TEST_FILE.open (TEST_FILE_NAME, std::ios_base::app);
    TEST_FILE << test.Structure() <<"," 
        << test.ElapsedTime() << ","
        << test.Operations()  << ","
        << test.ThreadAmount() << ","
        << test.Throughput() << ","
        << buckets << ","
        << reclamation << "\n";
    TEST_FILE.close();
}
void print_test(Test test, std::size_t buckets, std::string reclamation){

    std::cout << test.Structure() <<"," 
        << test.ElapsedTime() << ","
        << test.Operations()  << ","
        << test.ThreadAmount() << ","
        << test.Throughput() << ","
        << buckets << ","
        << reclamation << "\n";
}
template <typename Distribution, typename ...DistributionArgs>
void test(
    unsigned int max_n_threads, 
    unsigned long min_operations, 
    unsigned long max_operations, 
    std::size_t replications, 
    unsigned long pre_population, 
    double get_proportion, 
    double set_proportion, 
    double delete_proportion,
    DistributionArgs... distribution_args
    ){

    std::default_random_engine generator;
    clear_file();
    Distribution distribution(distribution_args...);

    for (size_t i = 0; i < replications; i++)
    {
        std::cout << "REP "<< i << "\n";
        for (unsigned int thread_amount = 2; thread_amount <= max_n_threads; thread_amount*=2)
        {
            std::cout << "THREADS "<< thread_amount << "\n";
            for (unsigned long operations = min_operations; operations < max_operations; operations*=10)
            {       
                std::cout << "OPERATIONS 10E"<< log10(operations) << "\n";
                    Test test = Test::LockUnorderedMap<
                        std::uniform_real_distribution<double>>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 0,"0");
                    print_test(test, 0,"0");
                    
                    test = Test::TBBMap<
                        std::uniform_real_distribution<double>>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 0,"0");
                    print_test(test, 0,"0");

                    test = Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::lock_free_ref_count<>,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 512,"lock_free_ref_count");
                    print_test(test, 512,"lock_free_ref_count");

            
                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::hazard_pointer<>,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 512,"hazard_pointer");
                    print_test(test, 512,"hazard_pointer");
                    
            
                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::hazard_eras<>,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 512,"hazard_eras");
                    print_test(test, 512,"hazard_eras");

                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::stamp_it,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 512,"stamp_it");
                    print_test(test, 512,"stamp_it");
                        
                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::quiescent_state_based,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution);
                    append_to_file(test, 512,"quiescent_state_based");
                    print_test(test, 512,"quiescent_state_based");  

                    test =  Test::WFCLabordeWaitFree<
                        std::uniform_real_distribution<double>>
                        (operations, thread_amount, 
                        pre_population, get_proportion, set_proportion, delete_proportion, distribution, 4);
                    append_to_file(test, 0,"0");
                    print_test(test, 0,"0");

	                typedef cds::gc::HP CDSHP;
                    test = Test::LibCDSFeldman<CDSHP, Distribution, DistributionArgs...>(
                        operations, thread_amount, pre_population, get_proportion, 
                        set_proportion, delete_proportion, distribution_args...);
                    print_test(test, 0,"HP");

	                typedef cds::gc::DHP CDSDHP;
                    test = Test::LibCDSFeldman<CDSDHP, Distribution, DistributionArgs...>(
                        operations, thread_amount, pre_population, get_proportion, 
                        set_proportion, delete_proportion, distribution_args...);
                    print_test(test, 0,"DHP");

            }
        }
    }
}

struct update_functor {
    // For FeldmanHashMap, IterableList
    template <typename Val>
    void operator()( Val& cur, Val * old )
    {}
};

int main(int argc, char *argv[]){
    
    unsigned int max_n_threads = 2;
    unsigned long min_operations = (unsigned long) 1E6;
    unsigned long max_operations = (unsigned long) 1E7;
    std::size_t replications = 1;

    unsigned long pre_population = (unsigned long) 1E4;

    double get_proportionn = 0.7;
    double set_proportion = 0.2;
    double delete_proportion = 0.1;

    //std::normal_distribution<double> normal_distribution(10000, 200);

    test<std::uniform_real_distribution<double>, double, double>(
        max_n_threads, 
        min_operations, 
        max_operations, 
        replications, 
        pre_population, 
        get_proportionn, 
        set_proportion, 
        delete_proportion,
        -1e9, 1e9
    );
}
