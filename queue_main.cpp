#include <iostream>
#include <random>
#include "queue_tests.cpp"
#include <xenium/michael_scott_queue.hpp>
#include <xenium/policy.hpp>
#include <xenium/reclamation/lock_free_ref_count.hpp>
#include <xenium/reclamation/hazard_pointer.hpp>
#include <xenium/reclamation/hazard_eras.hpp>
#include <xenium/reclamation/stamp_it.hpp>
#include <xenium/reclamation/quiescent_state_based.hpp>
#include <fstream>
#include <string>
#include <math.h>
#include <array>

#include <cds/gc/hp.h>
#include <cds/gc/dhp.h>

#ifdef BIG
typedef std::array<uint32_t, 1024> value_t;
#else
typedef uint32_t value_t;
#endif

void print_test(Test test, std::string text){

    std::cout << test.Structure() << text <<","
        << test.ThreadAmount() <<","
        << test.ElapsedTime() << "\n";
}

template <typename Distribution, typename ...DistributionArgs>
void test(
    int thread_amount, 
    long operations,
    long pre_population,
    long data_structure
    ){
    
    Test test;
    
    switch (data_structure)
    {
    case 1:
        test =  Test::STLQueue<value_t>(operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 2:
        test =  Test::NikolaevQueue<value_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 3:
        test =  Test::XeniumMSQueue<value_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 4:
        test =  Test::CDSMSQueue<value_t, 
            cds::gc::HP>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 5:
        test =  Test::TBB<value_t>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 6:
        test =  Test::BoostQueue<value_t>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 7:
        test =  Test::STLQueuePC<value_t>(operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 8:
        test =  Test::NikolaevPC<value_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 9:
        test =  Test::XeniumMSPC<value_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 10:
        test =  Test::CDSMSPC<value_t, 
            cds::gc::HP>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 11:
        test =  Test::TBBPC<value_t>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 12:
        test =  Test::BoostPC<value_t>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    default:
        break;
    }
}

const size_t THREAD_AMOUNT = 1;
const size_t OPERATIONS = 2;
const size_t PRE_POPULATION = 3;
const size_t DATA_STRUCTURE = 4;

int main(int argc, char *argv[]){
    
    test<boost::random::uniform_int_distribution<uint32_t>, long, long>(
        atoi(argv[THREAD_AMOUNT]),
        atol(argv[OPERATIONS]), 
        atol(argv[PRE_POPULATION]), 
        atol(argv[DATA_STRUCTURE])
    );
}
