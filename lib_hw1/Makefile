CC=gcc
CFLAGS=-I.

tester: list.o interface.o
	$(CC) -o tester list.o interface.o

list.o: list.c list.h
	$(CC) -c -o $@ $< $(CFLAGS)

interface.o: interface.c interface.h list.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o
	rm tester
