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
#include <math.h>

std::ofstream TEST_FILE;
std::string TEST_FILE_NAME = "test.csv";

void clear_file(){
    TEST_FILE.open (TEST_FILE_NAME);
    TEST_FILE << "Structure,Elapsed Time,Operations,Threads,Throughput,BucketAmount,Reclamation\n";
    TEST_FILE.close();
}
void append_to_file(Test test, std::size_t buckets, std::size_t node_size, std::string reclamation){

    TEST_FILE.open (TEST_FILE_NAME, std::ios_base::app);
    TEST_FILE << test.Structure() <<"," 
        << test.ElapsedTime() << ","
        << test.Operations()  << ","
        << test.ThreadAmount() << ","
        << test.Throughput() << ","
        << buckets << ","
        << node_size << ","
        << reclamation << "\n";
    TEST_FILE.close();
}
void print_test(Test test, std::size_t buckets, std::size_t node_size, std::string reclamation){

    std::cout << test.Structure() <<"," 
        << test.ElapsedTime() << ","
        << test.Operations()  << ","
        << test.ThreadAmount() << ","
        << test.Throughput() << ","
        << buckets << ","
        << node_size << ","
        << reclamation << "\n";
}
template <typename Distribution, typename ...DistributionArgs>
void test(
    int thread_amount, 
    long operations,
    long pre_population, 
    double get_proportion, 
    double set_proportion, 
    double delete_proportion,
    int data_structure,
    DistributionArgs... distribution_args
    ){

    std::default_random_engine generator;
    clear_file();
    Distribution distribution(distribution_args...);
    
    Test test;
    
    switch (data_structure)
    {
    case 1:
            test = Test::LockUnorderedMap<Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 0, 0,"0");
        print_test(test, 0, 0,"0");
        break;
    case 2:
        test = Test::TBBMap<Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 0, 0,"0");
        print_test(test, 0, 0,"0");
        break;
    case 3:
        test = Test::HarrisMichaelMapTest<
            xenium::reclamation::lock_free_ref_count<>,
            65536, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 65536, 0, "lock_free_ref_count");
        print_test(test, 65536, 0, "lock_free_ref_count");
        break;
    case 4:
        test =  Test::HarrisMichaelMapTest<
            xenium::reclamation::hazard_pointer<>,
            65536, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 65536, 0, "hazard_pointer");
        print_test(test, 65536, 0, "hazard_pointer");
        break;
    case 5:
        test = Test::HarrisMichaelMapTest<
            xenium::reclamation::lock_free_ref_count<>,
            65536, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 1024, 0, "lock_free_ref_count");
        print_test(test, 1024, 0, "lock_free_ref_count");
        break;
    case 6:
        test =  Test::HarrisMichaelMapTest<
            xenium::reclamation::hazard_pointer<>,
            1024, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 1024, 0, "hazard_pointer");
        print_test(test, 1024, 0, "hazard_pointer");
        break;
    case 7:
        test =  Test::WFCLabordeWaitFree<4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,4 , 0,"0");
        print_test(test, 4, 0,"0");
        break;
    case 8:
        test =  Test::WFCLabordeWaitFree<16, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, 16, 0,"0");
        print_test(test, 16, 0,"0");
        break;
    case 9:
        test =  Test::LibCDSFeldman<cds::gc::HP, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,0, 0,"hp");
        print_test(test,0, 0,"hp");
        break;
    case 10:
        test =  Test::LibCDSFeldman<cds::gc::DHP, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,0, 0,"dhp");
        print_test(test,0, 0,"dhp");
        break;
    case 11:
        test =  Test::LibCDSMichael<cds::gc::HP, 4294967296-1, 65536, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,0, 0,"hp");
        print_test(test,0, 0,"hp");
        break;
    case 12:
        test =  Test::LibCDSMichael<cds::gc::DHP, 4294967296-1, 65536, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,0, 0,"dhp");
        print_test(test,0, 0,"dhp");
        break;
    default:
        break;
    }
}

const size_t THREAD_AMOUNT = 1;
const size_t OPERATIONS = 2;
const size_t PRE_POPULATION = 3;
const size_t GET_PROPORTION = 4;
const size_t SET_PROPORTION = 5;
const size_t DELETE_PROPORTION = 6;
const size_t DATA_STRUCTURE = 7;

const size_t XENIUM_HARRIS_MICHAEL_BUCKETS = 1;
const size_t CDS_MICHAEL_MAX_ITEM_COUNT = 2;
const size_t CDS_MICHAEL_LOAD_FACTOR = 3;

int main(int argc, char *argv[]){

    test<std::uniform_real_distribution<double>, double, double>(
        atoi(argv[THREAD_AMOUNT]), 
        atol(argv[OPERATIONS]), 
        atol(argv[PRE_POPULATION]), 
        atof(argv[GET_PROPORTION]), 
        atof(argv[SET_PROPORTION]), 
        atof(argv[DELETE_PROPORTION]),
        atof(argv[DATA_STRUCTURE]),
        -1e9, 1e9
    );
}
