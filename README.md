# Using-Cpp-Parallel-Programming

how you compile OpenMP on a mac:
 g++  -Xpreprocessor  -fopenmp   -I/opt/homebrew/Cellar/libomp/14.0.0/include  -L/opt/homebrew/Cellar/libomp/14.0.0/lib  -lomp   *.cpp  -o  main
 
Stackoverflow reports that this is how to compile for OpenMP on a Mac:

clang  -Xpreprocessor  -fopenmp  -I/usr/local/include  -L/usr/local/lib  -lomp    main.c  -o  main

and

clang++  -Xpreprocessor  -fopenmp  -I/usr/local/include  -L/usr/local/lib  -lomp    main.cpp  -o  main
