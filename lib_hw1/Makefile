main.out: main.o main.o hash.o list.o 	
	gcc -g -o main.out main.o hash.o list.o 
main.o: main.c main.h bitmap.h debug.h limits.h list.h round.h
	gcc -g -c main.c -o main.o
hash.o: hash.c hash.h
	gcc -g -c hash.c -o hash.o
list.o: list.c list.h 
	gcc -g -c list.c -o list.o
clean: 
	-rm *.o
	-rm main.out
