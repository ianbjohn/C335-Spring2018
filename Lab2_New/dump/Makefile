CC = gcc
CFLAGS = -g
LDFLAGS = -g


Dump: poly.o dump.o
	$(CC) $(LDFLAGS) -o Dump poly.o dump.o

poly.o: poly.c
	$(CC) $(CFLAGS) -c poly.c
dump.o: dump.c
	$(CC) $(CFLAGS) -c dump.c
