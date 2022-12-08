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
    unsigned int thread_amount, 
    unsigned long operations,
    unsigned long pre_population, 
    double get_proportion, 
    double set_proportion, 
    double delete_proportion,
    DistributionArgs... distribution_args
    ){

    std::default_random_engine generator;
    clear_file();
    Distribution distribution(distribution_args...);

    std::cout << "OPERATIONS 10E"<< log10(operations) << "\n";
    Test test = Test::LockUnorderedMap<Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 0,"0");
    print_test(test, 0,"0");
    
    test = Test::TBBMap<Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 0,"0");
    print_test(test, 0,"0");

    test = Test::HarrisMichaelMapTest<
        xenium::reclamation::lock_free_ref_count<>,
        512, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 512,"lock_free_ref_count");
    print_test(test, 512,"lock_free_ref_count");


    test =  Test::HarrisMichaelMapTest<
        xenium::reclamation::hazard_pointer<>,
        512, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 512,"hazard_pointer");
    print_test(test, 512,"hazard_pointer");
    

    test =  Test::HarrisMichaelMapTest<
        xenium::reclamation::hazard_eras<>,
        512, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 512,"hazard_eras");
    print_test(test, 512,"hazard_eras");

    test =  Test::HarrisMichaelMapTest<
        xenium::reclamation::stamp_it,
        512, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 512,"stamp_it");
    print_test(test, 512,"stamp_it");
        
    test =  Test::HarrisMichaelMapTest<
        xenium::reclamation::quiescent_state_based,
        512, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 512,"quiescent_state_based");
    print_test(test, 512,"quiescent_state_based");  

    test =  Test::WFCLabordeWaitFree<4, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 0,"0");
    print_test(test, 0,"0");

    
    test =  Test::LibCDSFeldman<cds::gc::HP, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 0,"hp");
    print_test(test, 0,"hp");
    
    test =  Test::LibCDSFeldman<cds::gc::DHP, Distribution, DistributionArgs...>
        (operations, thread_amount, pre_population, get_proportion, 
        set_proportion, delete_proportion, distribution_args...);
    append_to_file(test, 0,"dhp");
    print_test(test, 0,"dhp");

}

int main(int argc, char *argv[]){
    
    unsigned int thread_amount = 16;
    unsigned long operations = (unsigned long) 1E7;

    unsigned long pre_population = (unsigned long) 1E4;

    double get_proportionn = 0.7;
    double set_proportion = 0.2;
    double delete_proportion = 0.1;

    test<std::uniform_real_distribution<double>, double, double>(
        thread_amount, 
        operations, 
        pre_population, 
        get_proportionn, 
        set_proportion, 
        delete_proportion,
        -1e9, 1e9
    );
}
