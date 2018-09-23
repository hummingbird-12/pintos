CC=gcc
CFLAGS=-I.

tester: list.o hash.o interface.o
	$(CC) -g -Wall -o tester list.o interface.o

list.o: list.c list.h
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

hash.o: hash.c hash.h list.h
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

interface.o: interface.c interface.h list.h hash.h
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

clean:
	rm *.o
	rm tester
