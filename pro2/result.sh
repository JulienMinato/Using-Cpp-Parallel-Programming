#!/bin/bash

touch pro2data.csv 
rm pro2data.csv

echo "Number of Threads, Number of Numnodes, the total volume, Megatrials per Second" >> pro2data.csv 

# number of threads:
for t in 1 2 4 8 16 32 64 128 
do
	echo NUMT = $t
	# number of Numnodes:
	for n in 4 32 128 500 800 1000 1500 2000 2500 3000 4000 5000 6000 7000

	do
		echo NUMNODES = $n
		g++ -DNUMNODES=$n -DNUMT=$t pro2.cpp -o pro2 -lm -fopenmp
		./pro2 >> pro2data.csv
	done
    echo -e >> pro2data.csv
done


echo "The results have been saved in pro2data.csv"