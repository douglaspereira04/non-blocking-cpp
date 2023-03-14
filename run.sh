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
#few_keys_range=1000
#many_keys_range=100000
#operations=50000000

few_keys_range=1000
many_keys_range=1000000
operations=10000000

for k in {1..20}; do

    for j in 4 8 16 32; do
        
        #a 90 5 5

        #vazio - poucas chaves - valores pequenas
        for i in 2 3 5 7 11; do 
            ./main $j $operations  0 0.9 0.05 0.05 $i $few_keys_range vazio_poucas_pequenas_a.csv
        done

        #vazio - muitas chaves - valores pequenas
        for i in 2 4 5 7 12; do 
            ./main $j $operations  0 0.9 0.05 0.05 $i $many_keys_range vazio_muitas_pequenas_a.csv
        done

        #prepopulado - poucas chaves - valores pequenas
        for i in 2 3 5 7 11; do 
            ./main $j $operations  $few_keys_range 0.9 0.05 0.05 $i $few_keys_range cheio_poucas_pequenas_a.csv
        done

        #prepopulado - muitas chaves - valores pequenas
        for i in 2 4 5 7 12; do 
            ./main $j $operations  $many_keys_range 0.9 0.05 0.05 $i $many_keys_range cheio_muitas_pequenas_a.csv
        done

        #vazio - poucas chaves - valores grandes
        for i in 15 16 20 24; do 
            ./main $j $operations  0 0.9 0.05 0.05 $i $few_keys_range vazio_poucas_grandes_a.csv
        done

        #vazio - muitas chaves - valores grandes
        for i in 15 17 20 25; do 
            ./main $j $operations  0 0.9 0.05 0.05 $i $many_keys_range vazio_muitas_grandes_a.csv
        done

        #prepopulado - poucas chaves - valores grandes
        for i in 15 16 20 24; do 
            ./main $j $operations  $few_keys_range 0.9 0.05 0.05 $i $few_keys_range cheio_poucas_grandes_a.csv
        done

        #prepopulado - muitas chaves - valores grandes
        for i in 15 17 20 25; do 
            ./main $j $operations  $many_keys_range 0.9 0.05 0.05 $i $many_keys_range cheio_muitas_grandes_a.csv
        done


        #b 45 45 10

        #vazio - poucas chaves - valores pequenas
        for i in 2 3 5 7 11; do 
            ./main $j $operations  0 0.45 0.45 0.10 $i $few_keys_range vazio_poucas_pequenas_b.csv
        done

        #vazio - muitas chaves - valores pequenas
        for i in 2 4 5 7 12; do 
            ./main $j $operations  0 0.45 0.45 0.10  $i $many_keys_range vazio_muitas_pequenas_b.csv
        done

        #prepopulado - poucas chaves - valores pequenas
        for i in 2 3 5 7 11; do 
            ./main $j $operations  $few_keys_range 0.45 0.45 0.10  $i $few_keys_range cheio_poucas_pequenas_b.csv
        done

        #prepopulado - muitas chaves - valores pequenas
        for i in 2 4 5 7 12; do 
            ./main $j $operations  $many_keys_range 0.45 0.45 0.10  $i $many_keys_range cheio_muitas_pequenas_b.csv
        done

        #vazio - poucas chaves - valores grandes
        for i in 15 16 20 24; do 
            ./main $j $operations  0 0.45 0.45 0.10  $i $few_keys_range vazio_poucas_grandes_b.csv
        done

        #vazio - muitas chaves - valores grandes
        for i in 15 17 20 25; do 
            ./main $j $operations  0 0.45 0.45 0.10  $i $many_keys_range vazio_muitas_grandes_b.csv
        done

        #prepopulado - poucas chaves - valores grandes
        for i in 15 16 20 24; do 
            ./main $j $operations  $few_keys_range 0.45 0.45 0.10  $i $few_keys_range cheio_poucas_grandes_b.csv
        done

        #prepopulado - muitas chaves - valores grandes
        for i in 15 17 20 25; do 
            ./main $j $operations  $many_keys_range 0.45 0.45 0.10  $i $many_keys_range cheio_muitas_grandes_b.csv
        done

        #c 5 90 5

        #vazio - poucas chaves - valores pequenas
        for i in 2 3 5 7 11; do 
            ./main $j $operations  0 0.05 0.9 0.05  $i $few_keys_range vazio_poucas_pequenas_c.csv
        done

        #vazio - muitas chaves - valores pequenas
        for i in 2 4 5 7 12; do 
            ./main $j $operations  0 0.05 0.9 0.05 $i $many_keys_range vazio_muitas_pequenas_c.csv
        done

        #prepopulado - poucas chaves - valores pequenas
        for i in 2 3 5 7 11; do 
            ./main $j $operations  $few_keys_range 0.05 0.9 0.05 $i $few_keys_range cheio_poucas_pequenas_c.csv
        done

        #prepopulado - muitas chaves - valores pequenas
        for i in 2 4 5 7 12; do 
            ./main $j $operations  $many_keys_range 0.05 0.9 0.05 $i $many_keys_range cheio_muitas_pequenas_c.csv
        done

        #vazio - poucas chaves - valores grandes
        for i in 15 16 20 24; do 
            ./main $j $operations  0 0.05 0.9 0.05 $i $few_keys_range vazio_poucas_grandes_c.csv
        done

        #vazio - muitas chaves - valores grandes
        for i in 15 17 20 25; do 
            ./main $j $operations  0 0.05 0.9 0.05 $i $many_keys_range vazio_muitas_grandes_c.csv
        done

        #prepopulado - poucas chaves - valores grandes
        for i in 15 16 20 24; do 
            ./main $j $operations  $few_keys_range 0.05 0.9 0.05 $i $few_keys_range cheio_poucas_grandes_c.csv
        done

        #prepopulado - muitas chaves - valores grandes
        for i in 15 17 20 25; do 
            ./main $j $operations  $many_keys_range 0.05 0.9 0.05 $i $many_keys_range cheio_muitas_grandes_c.csv
        done


    done
done