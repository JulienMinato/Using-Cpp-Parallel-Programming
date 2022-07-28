#!/bin/bash

# Local work Size
echo -e "GPU MULTIPLY 2 ARRAYS"
echo -e "NUM_ELEMENTS\tLOCAL_SIZE\tNUM_WORK_GROUPS\t\tPerformance\n"
for local_size in 2 32 128 256 512 1024
do
	# Global Work Size
	for global_size in 1 2 4 6 8 10 16 24 32 64 128
	do
		g++ -DNUM_ELEMENTS=$((global_size*1024*1024)) -DLOCAL_SIZE=$local_size -DMULT -o firstP1 firstPart1.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -Wno-write-strings
		./firstP1
	done
done

echo -e "\nGPU MULTIPLY AND ADD"
for local_size in 2 32 128 256 512 1024
do
	# Global Work Size
	for global_size in 1 2 4 6 8 10 16 24 32 64 128
	do
		g++ -DNUM_ELEMENTS=$((global_size*1024*1024)) -DLOCAL_SIZE=$local_size -DMULT_ADD -o firstP1 firstPart1.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -Wno-write-strings

		./firstP1
	done
done

echo -e "\nGPU MULTIPLY-REDUCE"

# Global Work Size
for global_size in 1 2 4 6 8 10 16 24 32 64 128
do
	g++ -DNMB=$global_size -o firstP2 firstPart2.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -Wno-write-strings
	./firstP2
done

