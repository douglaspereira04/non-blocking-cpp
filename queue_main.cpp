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
        test =  Test::STLQueue<uint32_t>(operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 2:
        test =  Test::NikolaevQueue<uint32_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 3:
        test =  Test::XeniumMSQueue<uint32_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 4:
        test =  Test::CDSMSQueue<uint32_t, 
            cds::gc::HP>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 5:
        test =  Test::CDSBasketQueue<uint32_t, 
            cds::gc::HP>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 6:
        test =  Test::TBB<uint32_t>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 7:
        test =  Test::BoostQueue<uint32_t>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 8:
        test =  Test::STLQueue<std::array<uint32_t,1024>>(operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 9:
        test =  Test::NikolaevQueue<std::array<uint32_t,1024>, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 10:
        test =  Test::XeniumMSQueue<std::array<uint32_t,1024>, 
            xenium::reclamation::hazard_pointer<>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 11:
        test =  Test::CDSMSQueue<std::array<uint32_t,1024>, 
            cds::gc::DHP>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 12:
        test =  Test::CDSBasketQueue<std::array<uint32_t,1024>, 
            cds::gc::DHP>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 13:
        test =  Test::TBB<std::array<uint32_t,1024>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 14:
        test =  Test::BoostQueue<std::array<uint32_t,1024>>
            (operations, thread_amount, pre_population);
        print_test(test, "");
        break;
    case 15:
        test =  Test::STLQueueSPSC<uint32_t>(operations, pre_population);
        print_test(test, "");
        break;
    case 16:
        test =  Test::NikolaevSPSC<uint32_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 17:
        test =  Test::XeniumMSSPSC<uint32_t, 
            xenium::reclamation::hazard_pointer<>>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 18:
        test =  Test::CDSMSSPSC<uint32_t, 
            cds::gc::HP>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 19:
        test =  Test::CDSBasketSPSC<uint32_t, 
            cds::gc::HP>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 20:
        test =  Test::TBBSPSC<uint32_t>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 21:
        test =  Test::BoostSPSC<uint32_t>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 22:
        test =  Test::STLQueueSPSC<std::array<uint32_t,1024>>(operations, pre_population);
        print_test(test, "");
        break;
    case 23:
        test =  Test::NikolaevSPSC<std::array<uint32_t,1024>, 
            xenium::reclamation::hazard_pointer<>>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 24:
        test =  Test::XeniumMSSPSC<std::array<uint32_t,1024>, 
            xenium::reclamation::hazard_pointer<>>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 25:
        test =  Test::CDSMSSPSC<std::array<uint32_t,1024>, 
            cds::gc::HP>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 26:
        test =  Test::CDSBasketSPSC<std::array<uint32_t,1024>, 
            cds::gc::HP>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 27:
        test =  Test::TBBSPSC<std::array<uint32_t,1024>>
            (operations, pre_population);
        print_test(test, "");
        break;
    case 28:
        test =  Test::BoostSPSC<std::array<uint32_t,1024>>
            (operations, pre_population);
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
