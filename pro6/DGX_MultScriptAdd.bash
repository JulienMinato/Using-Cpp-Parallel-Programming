#!/bin/bash
#SBATCH -J Mult
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o MultAdd.out
#SBATCH -e MultAdd.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=zhaolia@oregonstate.edu

echo -e "NUM_ELEMENTS\tLOCAL_SIZE\tNUM_WORK_GROUPS\t\tPerformance\n"

for local_size in 2 4 8 32 128 256 512 1024
do

    for global_size in 1 2 4 8 16 32 64 128
    do
        g++ -DNUM_ELEMENTS=$((global_size*1024*1024)) -DLOCAL_SIZE=$local_size -o MultAdd MultAdd.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -Wno-write-strings
        ./MultAdd
    done
done