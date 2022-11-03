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
#include <cds/container/feldman_hashmap_dhp.h>
#include <cds/gc/hp.h>

const std::size_t Buckets = 256;
int main(int argc, char *argv[]){
    
    unsigned int n_threads = 32;

    unsigned long operations = (unsigned long) 1E7;

    unsigned long pre_population = (unsigned long) 1E4;

    double get_proportionn = 0.7;
    double set_proportion = 0.2;
    double delete_proportion = 0.1;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(10000, 200);

    Test::LockUnorderedMap<
        std::normal_distribution<double>>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

    Test::TBBMap<
        std::normal_distribution<double>>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::lock_free_ref_count<>,
        4096>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);
        
    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::hazard_pointer<>,
        4096>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);
        
    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::hazard_eras<>,
        4096>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);
        
    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::stamp_it,
        4096>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::quiescent_state_based,
        4096>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

    Test::Vyukov<
        std::normal_distribution<double>, 
        xenium::reclamation::hazard_pointer<>>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

    Test::Vyukov<
        std::normal_distribution<double>, 
        xenium::reclamation::hazard_eras<>>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

    Test::Vyukov<
        std::normal_distribution<double>, 
        xenium::reclamation::hazard_eras<>>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);


    typedef cds::gc::HP gc_type;
    typedef cds::container::FeldmanHashMap< gc_type, int, int, cds::container::feldman_hashmap::traits> feldman_map;
    feldman_map map;

}