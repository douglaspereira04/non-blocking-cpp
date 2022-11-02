#include <iostream>
#include <random>
#include "test.cpp"

const std::size_t Buckets = 256;
int main(int argc, char *argv[]){
    
    unsigned int n_threads = 4;

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
        128>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);
        
    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::lock_free_ref_count<>,
        512>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);
        
    Test::HarrisMichaelMapTest<
        std::normal_distribution<double>, 
        xenium::reclamation::lock_free_ref_count<>,
        1024>
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
        xenium::reclamation::lock_free_ref_count<>,
        4096>
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
        xenium::reclamation::lock_free_ref_count<>,
        4096>
        (operations, n_threads, 
        pre_population, get_proportionn, set_proportion, delete_proportion, distribution);

}