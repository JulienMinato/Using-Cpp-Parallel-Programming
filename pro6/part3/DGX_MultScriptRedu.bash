#!/bin/bash
#SBATCH -J Mult
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o MultRedu.out
#SBATCH -e MultRedu.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=zhaolia@oregonstate.edu

echo -e "\nGPU MULTIPLY-REDUCE"

# Global Work Size
for global_size in 1 2 4 6 8 10 16 24 32 64 128
do
	g++ -DNMB=$global_size -o firstP2 firstPart2.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -Wno-write-strings
	./firstP2
done
