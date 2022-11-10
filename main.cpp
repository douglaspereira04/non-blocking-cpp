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

#include <cds/container/feldman_hashmap_dhp.h>
#include <cds/gc/hp.h>

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

const std::size_t Buckets = 256;
int main(int argc, char *argv[]){
    
    unsigned int max_n_threads = 64;
    unsigned long max_operations = (unsigned long) 1E7;
    std::size_t replications = 1;

    unsigned long pre_population = (unsigned long) 1E4;

    double get_proportionn = 0.7;
    double set_proportion = 0.2;
    double delete_proportion = 0.1;

    std::default_random_engine generator;
    std::normal_distribution<double> normal_distribution(10000, 200);
    std::uniform_real_distribution<double> uniform_distribution(-1e9, 1e9);

    clear_file();

    for (size_t i = 0; i < replications; i++)
    {
        
        for (unsigned int thread_amount = 2; thread_amount <= max_n_threads; thread_amount*=2)
        {
            for (unsigned long operations = (unsigned long) 1E4; operations < max_operations; operations*=10)
            {       
                    Test test = Test::LockUnorderedMap<
                        std::uniform_real_distribution<double>>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 0,"0");

                    /*test = Test::TBBMap<
                        std::uniform_real_distribution<double>>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 0,"0");*/


                    test = Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::lock_free_ref_count<>,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 512,"lock_free_ref_count");

            
                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::hazard_pointer<>,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 512,"hazard_pointer");
                    
            
                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::hazard_eras<>,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 512,"hazard_eras");

                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::stamp_it,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 512,"stamp_it");
                        
                    test =  Test::HarrisMichaelMapTest<
                        std::uniform_real_distribution<double>, 
                        xenium::reclamation::quiescent_state_based,
                        512>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution);
                    append_to_file(test, 512,"quiescent_state_based");  

                    test =  Test::WFCLabordeWaitFree<
                        std::uniform_real_distribution<double>>
                        (operations, thread_amount, 
                        pre_population, get_proportionn, set_proportion, delete_proportion, uniform_distribution, 4);
                    append_to_file(test, 0,"0");
            }
        }
    }
    
    
    
    typedef cds::gc::HP gc_type;
    typedef cds::container::FeldmanHashMap< gc_type, int, int, cds::container::feldman_hashmap::traits> feldman_map;
    feldman_map map;

}