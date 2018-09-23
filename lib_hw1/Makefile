main.out: main.o main.o hash.o list.o bitmap.o
	gcc -o main.out main.o hash.o list.o bitmap.o
main.o: main.c main.h bitmap.h debug.h limits.h list.h round.h
	gcc -c  main.c -o main.o
hash.o: hash.c hash.h
	gcc -c hash.c -o hash.o
list.o: list.c list.h 
	gcc -c list.c -o list.o
bitmap.o: bitmap.c bitmap.h
	gcc -c bitmap.c -o bitmap.o
clean: 
	-rm *.o
	-rm main.out
