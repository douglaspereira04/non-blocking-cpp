#!/bin/bash

echo "Structure,Threads,Elapsed Time" > small.csv
echo "Structure,Threads,Elapsed Time" > big.csv
echo "Structure,Threads,Elapsed Time" > small_spsc.csv
echo "Structure,Threads,Elapsed Time" > big_spsc.csv
#many_keys_range=100000
#operations=50000000

operations=10000000

for k in {1..10}; do

    for j in 1 2 4; do
    
        for i in 1 2 3; do 
            ./queue_main $j $operations 0 $i small.csv
        done
        
        for i in 4 5 6; do 
            ./queue_main $j $operations 0 $i big.csv
        done

    done
    
    for i in 7 8 9; do 
        ./queue_main $j $operations 0 $i small_spsc.csv
    done

    for i in 10 11 12; do 
        ./queue_main $j $operations 0 $i big_spsc.csv
    done

done