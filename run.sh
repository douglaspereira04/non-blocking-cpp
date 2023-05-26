#!/bin/bash

echo "Structure,Threads,Elapsed Time" > vazio_poucas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > vazio_muitas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > cheio_poucas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > cheio_muitas_pequenas_a.csv
echo "Structure,Threads,Elapsed Time" > vazio_poucas_grandes_a.csv
echo "Structure,Threads,Elapsed Time" > vazio_muitas_grandes_a.csv
echo "Structure,Threads,Elapsed Time" > cheio_poucas_grandes_a.csv
echo "Structure,Threads,Elapsed Time" > cheio_muitas_grandes_a.csv

echo "Structure,Threads,Elapsed Time" > vazio_poucas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > vazio_muitas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > cheio_poucas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > cheio_muitas_pequenas_b.csv
echo "Structure,Threads,Elapsed Time" > vazio_poucas_grandes_b.csv
echo "Structure,Threads,Elapsed Time" > vazio_muitas_grandes_b.csv
echo "Structure,Threads,Elapsed Time" > cheio_poucas_grandes_b.csv
echo "Structure,Threads,Elapsed Time" > cheio_muitas_grandes_b.csv

echo "Structure,Threads,Elapsed Time" > vazio_poucas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > vazio_muitas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > cheio_poucas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > cheio_muitas_pequenas_c.csv
echo "Structure,Threads,Elapsed Time" > vazio_poucas_grandes_c.csv
echo "Structure,Threads,Elapsed Time" > vazio_muitas_grandes_c.csv
echo "Structure,Threads,Elapsed Time" > cheio_poucas_grandes_c.csv
echo "Structure,Threads,Elapsed Time" > cheio_muitas_grandes_c.csv

few_keys_range=1000
many_keys_range=1000000
operations=10000000
search=(0.90 0.45 0.05)
insert=(0.05 0.45 0.90)
remove=(0.05 0.10 0.05)
names=(a b c)
few_keys_tests=(1 2 3 4 5 6 7 8 9)
many_keys_tests=(1 2 3 4 5 6 10 11 12)

for k in {1..20}; do
#rep
    for j in 1 2 4 8 12 16 24 32 48; do
    #threads
        for l in {0..2}; do
        #distribution

            #few keys
                #empty
                filename="vazio_poucas_pequenas_${names[l]}.csv"
                for i in "${few_keys_tests[@]}"; do 
                    ./main $j $operations  0 ${search[l]} ${insert[l]} ${remove[l]} $i $few_keys_range $filename
                done

                #prepopulated
                filename="cheio_poucas_pequenas_${names[l]}.csv"
                for i in "${few_keys_tests[@]}"; do 
                    ./main $j $operations  $few_keys_range ${search[l]} ${insert[l]} ${remove[l]} $i $few_keys_range $filename
                done
            
            #many keys
                #empty
                filename="vazio_muitas_pequenas_${names[l]}.csv"
                for i in "${many_keys_tests[@]}"; do 
                    ./main $j $operations  0 ${search[l]} ${insert[l]} ${remove[l]} $i $many_keys_range $filename
                done

                #prepopulated
                filename="cheio_muitas_pequenas_${names[l]}.csv"
                for i in "${many_keys_tests[@]}"; do 
                    ./main $j $operations  $many_keys_range ${search[l]} ${insert[l]} ${remove[l]} $i $many_keys_range $filename
                done

        done
    done
done