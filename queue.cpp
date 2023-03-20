#include <iostream>
#include <random>
#include "queue_test.cpp"
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
    int data_structure
    ){
    
    Test test;
    
    switch (data_structure)
    {
    case 1:
        test =  Test::MichaelScottQueue<uint32_t, 
            xenium::reclamation::hazard_pointer<>,
            true , 1, 0>
            (operations, thread_amount, pre_population);
        append_to_file(test, "");
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
const size_t FILE_NAME = 5;

int main(int argc, char *argv[]){
    TEST_FILE_NAME = argv[FILE_NAME];
    
    test<boost::random::uniform_int_distribution<uint32_t>, long, long>(
        atoi(argv[THREAD_AMOUNT]), 
        atol(argv[OPERATIONS]), 
        atol(argv[PRE_POPULATION]), 
        atof(argv[DATA_STRUCTURE])
    );
}
