CFLAGS = -std=c99 -g -pedantic -Wall
CC = gcc

all: hash.o hash.h htest1

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c -o hash.o

htest.o: htest1.c
	$(CC) $(CFLAGS) -c htest1.c -o htest1.o

htest1: htest1.o hash.o hash.h
	$(CC) $(CFLAGS) htest1.o hash.o -o htest1

clean:
	rm *.o
