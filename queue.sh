#!/bin/bash
mkdir -p results/queue

echo "Structure,Threads,Elapsed Time" > results/queue/scalability_empty_small.csv
echo "Structure,Threads,Elapsed Time" > results/queue/scalability_filled_small.csv
echo "Structure,Threads,Elapsed Time" > results/queue/scalability_empty_big.csv
echo "Structure,Threads,Elapsed Time" > results/queue/scalability_filled_big.csv

scalability_tests=(1 2 3 4 5 6)
scalability_threads=(1 2 4 8 16 32)

operations=10000000
insert_proportion=0.5

reps=10

make clean
cmake . -DBIG_VALUES=OFF
make -j4

for k in $(seq $reps); do
#rep
    for j in "${scalability_threads[@]}"; do
    #threads
        #scalability
            #empty
            echo "REP: ${k}; ${j}_scalability_empty_small"
            filename="scalability_empty_small.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                output=$(./queue $j $operations $operations $i $insert_proportion)
                echo $output
                echo $output >> results/queue/$filename
            done

            #filled
            echo "REP: ${k}; ${j}_scalability_filled_small"
            filename="scalability_filled_small.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                output=$(./queue $j $operations $operations $i $insert_proportion)
                echo $output
                echo $output >> results/queue/$filename
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
            echo "REP: ${k}; ${j}_scalability_empty_big"
            filename="scalability_empty_big.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                output=$(./queue $j $operations $operations $i $insert_proportion)
                echo $output
                echo $output >> results/queue/$filename
            done

            #filled
            echo "REP: ${k}; ${j}_scalability_filled_big"
            filename="scalability_filled_big.csv"
            for i in "${scalability_tests[@]}"; do 
                echo "${i}"
                output=$(./queue $j $operations $operations $i $insert_proportion)
                echo $output
                echo $output >> results/queue/$filename
            done
    done
done