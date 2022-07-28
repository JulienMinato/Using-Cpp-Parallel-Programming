#!/bin/bash


echo -e "NUM_ELEMENTS\tLOCAL_SIZE\tNUM_WORK_GROUPS\t\tPerformance\n"



for global_size in 1 2 4 8 16 32 64 128
do
    g++ -DNUM_ELEMENTS=$((global_size*1024*1024)) -o MultRedu MultRedu.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -Wno-write-strings
    ./MultRedu
done

