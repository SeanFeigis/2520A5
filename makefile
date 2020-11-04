CFLAGS = -std=c99 -pedantic -Wall
CC = gcc

all: hash.o hash.h

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c -o hash.o

htest1.o: htest1.c
	$(CC) $(CFLAGS) -c htest1.c -o htest1.o

htest1: htest1.o hash.o hash.h
	$(CC) $(CFLAGS) htest1.o hash.o -o htest1

htest2.o: htest2.c
	$(CC) $(CFLAGS) -c htest2.c -o htest2.o

htest2: htest2.o hash.o hash.h
	$(CC) $(CFLAGS) htest2.o hash.o -o htest2

htest3.o: htest3.c
	$(CC) $(CFLAGS) -c htest3.c -o htest3.o

htest3: htest3.o hash.o hash.h
	$(CC) $(CFLAGS) htest3.o hash.o -o htest3

htest4.o: htest4.c
	$(CC) $(CFLAGS) -c htest4.c -o htest4.o

htest4: htest4.o hash.o hash.h
	$(CC) $(CFLAGS) htest4.o hash.o -o htest4

clean:
	rm *.o
