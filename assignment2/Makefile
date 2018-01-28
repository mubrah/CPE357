CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
SOURCES = main.c hash.c word.c
OBJECTS = main.o hash.o word.o
.PHONY: clean

main : $(OBJECTS)
	$(CC) $(CFLAGS) -o fw $(OBJECTS)

main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c

word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c


clean:
	rm -f *.o

