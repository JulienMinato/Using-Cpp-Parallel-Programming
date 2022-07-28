#!/bin/bash

touch pro1data.csv 
rm pro1data.csv

echo "Number of Threads, Number of Trials, Probability of Hitting the Plate, Megatrials per Second" >> pro1data.csv 

# number of threads:
for t in 1 2 4 8 16 32 64 128
do
	echo NUMT = $t
	# number of trials:
	for s in 10000 100000 1000000 10000000 50000000 100000000

	do
		echo NUMTRIALS = $s
		g++ -DNUMTRIALS=$s -DNUMT=$t pro1.cpp -o pro1 -lm -fopenmp
		./pro1 >> pro1data.csv
	done
    echo -e >> pro1data.csv
done


echo "The results have been saved in pro1data.csv"