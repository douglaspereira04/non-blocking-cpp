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

#ifdef BIG
typedef std::array<uint32_t, 1024> KeyType;
#else
typedef uint32_t KeyType;
#endif

void append_to_file(Test test, std::string text){

    TEST_FILE.open (TEST_FILE_NAME, std::ios_base::app);
    TEST_FILE << test.Structure() << text <<","
        << test.ThreadAmount() << ","
        << test.ElapsedTime() << "\n";
    TEST_FILE.close();
}
void print_test(Test test, std::string text){

    std::cout << test.Structure() << text <<",\t"
        << test.ThreadAmount() << ",\t"
        << test.ElapsedTime() << "\n";
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
    std::cout << thread_amount
        << " " << operations
        << " " << pre_population
        << " " << get_proportion
        << " " << set_proportion
        << " " << delete_proportion
        << " " << data_structure 
        << std::endl;
    std::default_random_engine generator;
    //clear_file();
    Distribution distribution(distribution_args...);
    
    Test test;
    
    

    switch (data_structure)
    {
    case 1:
            test = Test::STDLock<KeyType, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 2:
        test = Test::TBBMap<KeyType, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 3:
        test =  Test::WFCUnorderedMap<KeyType, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 4:
        test =  Test::WFCUnorderedMap<KeyType, 8, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 8");
        print_test(test," 8");
        break;
    case 5:
        test =  Test::LibCDSFeldman<KeyType, cds::gc::HP,4,4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 6:
        test =  Test::LibCDSFeldman<KeyType, cds::gc::HP,8,8, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 8");
        print_test(test," 8");
        break;
    case 7:
        //expects few keys
        test =  Test::XeniumMichael<KeyType, 
            xenium::reclamation::hazard_pointer<>,
            1024 , Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, "");
        print_test(test, "");
        break;
    case 8:          
        //expects few keys                       
        test =  Test::LibCDSMichael<KeyType, cds::gc::HP, 1024, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 9:
        //expects few keys                       
        test =  Test::LibCDSSplitOrdered<KeyType, cds::gc::HP, 1024, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 10:       
        //expects more keys                              
        test =  Test::LibCDSMichael<KeyType, cds::gc::HP, 1048576, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 11:
        //expects more keys
        test =  Test::XeniumMichael<KeyType,
            xenium::reclamation::hazard_pointer<>,
            1048576, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, "");
        print_test(test, "");
        break;
    case 12:
        //expects more keys                 
        test =  Test::LibCDSSplitOrdered<KeyType, cds::gc::HP, 1048576, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
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
const size_t KEY_RANGE = 8;
const size_t FILE_NAME = 9;

int main(int argc, char *argv[]){
#ifdef BIG
std::cout << "BIG_VALUES" << std::endl;
#endif
    TEST_FILE_NAME = argv[FILE_NAME];
    
    test<boost::random::uniform_int_distribution<uint32_t>, long, long>(
        atoi(argv[THREAD_AMOUNT]), 
        atol(argv[OPERATIONS]), 
        atol(argv[PRE_POPULATION]), 
        atof(argv[GET_PROPORTION]), 
        atof(argv[SET_PROPORTION]), 
        atof(argv[DELETE_PROPORTION]),
        atof(argv[DATA_STRUCTURE]),
        0, atol(argv[KEY_RANGE])
    );
}
