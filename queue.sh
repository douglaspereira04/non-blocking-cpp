#!/bin/bash
mkdir -p results/queue

echo "Structure,Threads,Elapsed Time" > small.csv

operations=10000000

for k in {1..10}; do

    for j in 1 2 4; do

        for i in 1 2 3; do 
            ./queue $j $operations 0 $i >> small.csv
        done

    done

done