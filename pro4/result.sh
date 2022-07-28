#!/bin/bash

# number of threads:
for t in 1000 2000 4000 8000 10000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 5000000 6000000 8000000
do
	ARRAYSIZE=$t
	g++ -DARRAYSIZE=$t pro4.cpp -o prog4 -lm -fopenmp
	./prog4
done




