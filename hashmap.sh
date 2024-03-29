#!/bin/bash
mkdir -p results/hashmap
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_poucas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_muitas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_poucas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_muitas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_poucas_grandes_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_muitas_grandes_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_poucas_grandes_a.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_muitas_grandes_a.csv

echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_poucas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_muitas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_poucas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_muitas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_poucas_grandes_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_muitas_grandes_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_poucas_grandes_b.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_muitas_grandes_b.csv

echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_poucas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_muitas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_poucas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_muitas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_poucas_grandes_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/vazio_muitas_grandes_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_poucas_grandes_c.csv
echo "Structure,Threads,Elapsed Time" > results/hashmap/cheio_muitas_grandes_c.csv


few_keys_tests=(1 2 3 4 5 6 7 8 9)
many_keys_tests=(1 2 3 4 5 6 10 11 12)

few_keys_range=1024
many_keys_range=1048576

operations=10000000


search=(0.90 0.45 0.05)
insert=(0.05 0.45 0.90)
remove=(0.05 0.10 0.05)
names=(a b c)

threads=(1 4 8 16 32)
reps=3

make clean
cmake . -DBIG_VALUES=OFF
make -j4

for k in $(seq $reps); do
#rep
    for j in "${threads[@]}"; do
    #threads
        for l in {0..2}; do
        #distribution

            #few keys
                #empty
                echo "${j}_vazio_poucas_pequenas_${names[l]}"
                filename="vazio_poucas_pequenas_${names[l]}.csv"
                for i in "${few_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  0 ${search[l]} ${insert[l]} ${remove[l]} $i $few_keys_range >> results/hashmap/$filename
                done

                #prepopulated
                echo "${j}_cheio_poucas_pequenas_${names[l]}"
                filename="cheio_poucas_pequenas_${names[l]}.csv"
                for i in "${few_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  $few_keys_range ${search[l]} ${insert[l]} ${remove[l]} $i $few_keys_range >> results/hashmap/$filename
                done
            
            #many keys
                #empty
                echo "${j}_vazio_muitas_pequenas_${names[l]}"
                filename="vazio_muitas_pequenas_${names[l]}.csv"
                for i in "${many_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  0 ${search[l]} ${insert[l]} ${remove[l]} $i $many_keys_range >> results/hashmap/$filename
                done

                #prepopulated
                echo "${j}_cheio_muitas_pequenas_${names[l]}"
                filename="cheio_muitas_pequenas_${names[l]}.csv"
                for i in "${many_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  $many_keys_range ${search[l]} ${insert[l]} ${remove[l]} $i $many_keys_range >> results/hashmap/$filename
                done

        done
    done
done

make clean
cmake . -DBIG_VALUES=ON
make -j4

for k in $(seq $reps); do
#rep
    for j in "${threads[@]}"; do
    #threads
        for l in {0..2}; do
        #distribution

            #few keys
                #empty
                echo "${j}_vazio_poucas_grandes_${names[l]}"
                filename="vazio_poucas_grandes_${names[l]}.csv"
                for i in "${few_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  0 ${search[l]} ${insert[l]} ${remove[l]} $i $few_keys_range >> results/hashmap/$filename
                done

                #prepopulated
                echo "${j}_cheio_poucas_grandes_${names[l]}"
                filename="cheio_poucas_grandes_${names[l]}.csv"
                for i in "${few_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  $few_keys_range ${search[l]} ${insert[l]} ${remove[l]} $i $few_keys_range >> results/hashmap/$filename
                done
            
            #many keys
                #empty
                echo "${j}vazio_muitas_grandes_${names[l]}"
                filename="vazio_muitas_grandes_${names[l]}.csv"
                for i in "${many_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  0 ${search[l]} ${insert[l]} ${remove[l]} $i $many_keys_range >> results/hashmap/$filename
                done

                #prepopulated
                echo "${j}_cheio_muitas_grandes_${names[l]}"
                filename="cheio_muitas_grandes_${names[l]}.csv"
                for i in "${many_keys_tests[@]}"; do 
                    echo "${i}"
                    ./hashmap $j $operations  $many_keys_range ${search[l]} ${insert[l]} ${remove[l]} $i $many_keys_range >> results/hashmap/$filename
                done

        done
    done
done
