test:	hw3.o
	cc hw3.o -o test

hw3.o:	hw3.c
	cc -c hw3.c
