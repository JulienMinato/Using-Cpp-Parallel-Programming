all: test 

test: 
	sh script.sh

reduce: 
	sh reduce.sh

proj: 
	clear
	clear
	make clean
	g++ -o first firstPart1.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
	./first

clean:
	rm -f firstP1 firstP2
