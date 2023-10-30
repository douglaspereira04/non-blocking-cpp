#include <iostream>
#include <random>
#include "hashmap_tests.cpp"
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

#ifdef BIG
typedef std::array<uint32_t, 1024> value_t;
#else
typedef uint32_t value_t;
#endif

void print_test(Test test, std::string text){

    std::cout << test.Structure() << text <<","
        << test.ThreadAmount() << ","
        << test.ElapsedTime() << "\n";
}

template <typename dist_t, typename ...dist_args_t>
void test(
    int thread_amount, 
    long operations,
    long pre_population, 
    double get_proportion, 
    double set_proportion, 
    double delete_proportion,
    int data_structure,
    dist_args_t... distribution_args
    ){
    std::default_random_engine generator;

    dist_t distribution(distribution_args...);

    Test test;

    switch (data_structure)
    {
    case 1:
            test = Test::STDLock<value_t, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test,"");
        break;
    case 2:
        test = Test::TBBMap<value_t, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test,"");
        break;
    case 3:
        test =  Test::WFCUnorderedMap<value_t, 4, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test," 4");
        break;
    case 4:
        test =  Test::WFCUnorderedMap<value_t, 8, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test," 8");
        break;
    case 5:
        test =  Test::LibCDSFeldman<value_t, cds::gc::HP,4,4, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test," 4");
        break;
    case 6:
        test =  Test::LibCDSFeldman<value_t, cds::gc::HP,8,8, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test," 8");
        break;
    case 7:
        //expects few keys
        test =  Test::XeniumMichael<value_t, 
            xenium::reclamation::hazard_pointer<>,
            1024 , dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test, "");
        break;
    case 8:          
        //expects few keys                       
        test =  Test::LibCDSMichael<value_t, cds::gc::HP, 1024, 1, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test,"");
        break;
    case 9:
        //expects few keys                       
        test =  Test::LibCDSSplitOrdered<value_t, cds::gc::HP, 1024, 1, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test,"");
        break;
    case 10:       
        //expects more keys                              
        test =  Test::LibCDSMichael<value_t, cds::gc::HP, 1048576, 1, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test,"");
        break;
    case 11:
        //expects more keys
        test =  Test::XeniumMichael<value_t,
            xenium::reclamation::hazard_pointer<>,
            1048576, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test, "");
        break;
    case 12:
        //expects more keys                 
        test =  Test::LibCDSSplitOrdered<value_t, cds::gc::HP, 1048576, 1, dist_t, dist_args_t...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        print_test(test,"");
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

int main(int argc, char *argv[]){
    
    test<boost::random::uniform_int_distribution<uint32_t>, long, long>(
        atoi(argv[THREAD_AMOUNT]), 
        atol(argv[OPERATIONS]), 
        atol(argv[PRE_POPULATION]), 
        atof(argv[GET_PROPORTION]), 
        atof(argv[SET_PROPORTION]), 
        atof(argv[DELETE_PROPORTION]),
        atoi(argv[DATA_STRUCTURE]),
        0L, atol(argv[KEY_RANGE])
    );
}
