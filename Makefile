
CFLAGS=-std=c99 -Wall -pedantic

readable: readable.o
	gcc readable.o -o readable

readable.o: readable.c
	gcc -c $(CFLAGS) readable.c

clean:
		rm readable *.o
