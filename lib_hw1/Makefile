CC=gcc
CFLAGS=-I.

tester: list.o hash.o bitmap.o interface.o
	$(CC) -g -Wall -o $@ $^ $(CFLAGS)

list.o: list.c list.h
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

hash.o: hash.c hash.h list.h
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

bitmap.o: bitmap.c bitmap.h limits.h round.h stdio.c
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

interface.o: interface.c interface.h list.h hash.h bitmap.h
	$(CC) -g -Wall -c -o $@ $< $(CFLAGS)

clean:
	rm *.o
	rm tester
