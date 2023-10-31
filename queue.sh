#!/bin/bash
mkdir -p results/queue

echo "Structure,Threads,Elapsed Time" > results/queue/scalability_empty_small.csv
echo "Structure,Threads,Elapsed Time" > results/queue/scalability_empty_big.csv
echo "Structure,Threads,Elapsed Time" > results/queue/pc_empty_small.csv
echo "Structure,Threads,Elapsed Time" > results/queue/pc_empty_big.csv

scalability_tests=(1 2 3 4 5 6)
pc_tests=(7 8 9 10 11 12)

operations=1000

scalability_threads=(1 4 8 16 32)
pc_threads=(2 4 8 16 32)
reps=2

make clean
cmake . -DBIG_VALUES=OFF
make -j4

for k in $(seq $reps); do
#rep
    for j in "${scalability_threads[@]}"; do
    #threads
        #scalability
            #empty
            echo "${j}_scalability_empty_small"
            filename="scalability_empty_small.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations 0 $i >> results/queue/$filename
            done

            #filled
            echo "${j}_scalability_filled_small"
            filename="scalability_filled_small.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations $operations $i >> results/queue/$filename
            done
    done

    for j in "${pc_threads[@]}"; do

        #pc
            #empty
            echo "pc_empty_small"
            filename="pc_empty_small.csv"
            for i in "${pc_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations 0 $i >> results/queue/$filename
            done

            #filled
            echo "pc_filled_small"
            filename="pc_filled_small.csv"
            for i in "${pc_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations $operations $i >> results/queue/$filename
            done
    done
done


make clean
cmake . -DBIG_VALUES=ON
make -j4

for k in $(seq $reps); do
#rep
    for j in "${scalability_threads[@]}"; do
    #threads
        #scalability
            #empty
            echo "${j}_scalability_empty_big"
            filename="scalability_empty_big.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations 0 $i >> results/queue/$filename
            done

            #filled
            echo "${j}_scalability_filled_big"
            filename="scalability_filled_big.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations $operations $i >> results/queue/$filename
            done
    done
    for j in "${pc_threads[@]}"; do
        #pc
            #empty
            echo "pc_empty_big"
            filename="pc_empty_big.csv"
            for i in "${pc_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations 0 $i >> results/queue/$filename
            done

            #filled
            echo "pc_filled_big"
            filename="pc_filled_big.csv"
            for i in "${pc_tests[@]}"; do 
                echo "${i}"
                ./queue $j $operations $operations $i >> results/queue/$filename
            done
    done

done