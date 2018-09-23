CC=gcc
CFLAGS=-I.

testlib: list.o hash.o bitmap.o interface.o
	$(CC) -Wall -o $@ $^ $(CFLAGS)

list.o: list.c list.h
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

hash.o: hash.c hash.h list.h
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

bitmap.o: bitmap.c bitmap.h limits.h round.h stdio.c
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

interface.o: interface.c interface.h list.h hash.h bitmap.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o
	rm testlib
