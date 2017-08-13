all:
	g++ -g -lgomp -fopenmp trick.cpp

test:
	./a.out
