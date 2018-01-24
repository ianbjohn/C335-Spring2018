rpn:	main.o cell.o
	cc main.o cell.o -o rpn
main.o: main.c cell.h
	cc -c --std=c99 main.c
cell.o: cell.c cell.h
	cc -c cell.c
