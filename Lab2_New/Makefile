CC = gcc
CFLAGS = -g
LDFLAGS = -g


mywc: wc.o
	$(CC) $(LDFLAGS) -o mywc wc.o

wc.o: wc.c
	$(CC) $(CFLAGS) -c wc.c
