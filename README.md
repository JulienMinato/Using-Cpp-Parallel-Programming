# Using-Cpp-Parallel-Programming

how you compile OpenMP on a mac:

 g++  -Xpreprocessor  -fopenmp   -I/opt/homebrew/Cellar/libomp/14.0.0/include  -L/opt/homebrew/Cellar/libomp/14.0.0/lib  -lomp   *.cpp  -o  main
