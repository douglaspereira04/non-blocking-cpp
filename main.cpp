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

    std::default_random_engine generator;
    //clear_file();
    Distribution distribution(distribution_args...);
    
    Test test;
    
    switch (data_structure)
    {
    case 1:
            test = Test::LockUnordered<uint32_t, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 2:
        test = Test::TBBMap<uint32_t, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 3:
        //expects few keys
        test =  Test::XeniumHarrisMichael<uint32_t, 
            xenium::reclamation::hazard_pointer<>,
            500 , Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, "");
        print_test(test, "");
        break;
    case 4:
        //expects more keys
        test =  Test::XeniumHarrisMichael<uint32_t, 
            xenium::reclamation::hazard_pointer<>,
            500000 , Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, "");
        print_test(test, "");
        break;
    case 5:
        test =  Test::WFCUnorderedMap<uint32_t, 16, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 16");
        print_test(test," 16");
        break;
    case 6:
        test =  Test::WFCUnorderedMap<uint32_t, 8, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 8");
        print_test(test," 8");
        break;
    case 7:
        test =  Test::LibCDSFeldman<uint32_t, cds::gc::HP,8,4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 8:
        test =  Test::LibCDSFeldman<uint32_t, cds::gc::HP,8,8, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 8");
        print_test(test," 8");
        break;
    case 9:          
        //expects few                           
        test =  Test::LibCDSMichael<uint32_t, cds::gc::HP, 1000, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 10:                                     
        test =  Test::LibCDSMichael<uint32_t, cds::gc::HP, 1000000, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 11:   
        //expects few                                  
        test =  Test::LibCDSMichael<uint32_t, cds::gc::HP, 1000, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 12:                                     
        test =  Test::LibCDSMichael<uint32_t, cds::gc::HP, 1000000, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 13:                                     
        test =  Test::Bhhbazinga<uint32_t, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 14:
            test = Test::LockUnordered<std::array<uint32_t, 1024>, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 15:
        test = Test::TBBMap<std::array<uint32_t, 1024>, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 16:
        test =  Test::XeniumHarrisMichael<std::array<uint32_t, 1024>, 
            xenium::reclamation::hazard_pointer<>,
            500 , Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, "");
        print_test(test, "");
        break;
    case 17:
        test =  Test::XeniumHarrisMichael<std::array<uint32_t, 1024>, 
            xenium::reclamation::hazard_pointer<>,
            500000 , Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test, "");
        print_test(test, "");
        break;
    case 18:
        test =  Test::WFCUnorderedMap<std::array<uint32_t, 1024>, 16, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 16");
        print_test(test," 16");
        break;
    case 19:
        test =  Test::WFCUnorderedMap<std::array<uint32_t, 1024>, 8, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 8");
        print_test(test," 8");
        break;
    case 20:
        test =  Test::LibCDSFeldman<std::array<uint32_t, 1024>, cds::gc::HP,8,4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 21:
        test =  Test::LibCDSFeldman<std::array<uint32_t, 1024>, cds::gc::HP,8,8, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 8");
        print_test(test," 8");
        break;
    case 22:                                     
        test =  Test::LibCDSMichael<std::array<uint32_t, 1024>, cds::gc::HP, 1000, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 23:                                     
        test =  Test::LibCDSMichael<std::array<uint32_t, 1024>, cds::gc::HP, 1000, 1, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 1");
        print_test(test," 1");
        break;
    case 24:                                     
        test =  Test::LibCDSMichael<std::array<uint32_t, 1024>, cds::gc::HP, 1000, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 25:                                     
        test =  Test::LibCDSMichael<std::array<uint32_t, 1024>, cds::gc::HP, 1000000, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
    case 26:                                     
        test =  Test::Bhhbazinga<std::array<uint32_t, 1024>, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test,"");
        print_test(test,"");
        break;
    case 27:
        test =  Test::WFCUnorderedMap<uint32_t, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
        break;
        break;
    case 28:
        test =  Test::WFCUnorderedMap<std::array<uint32_t, 1024>, 4, Distribution, DistributionArgs...>
            (operations, thread_amount, pre_population, get_proportion, 
            set_proportion, delete_proportion, distribution_args...);
        append_to_file(test," 4");
        print_test(test," 4");
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
