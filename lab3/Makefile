CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
SOURCES = htable.c words.c tree.c
OBJECTS = htable.o words.o tree.o
.PHONY: clean

main : $(OBJECTS)
	$(CC) $(CFLAGS) -o htable $(OBJECTS)

htable.o: htable.c htable.h words.h tree.h
	$(CC) $(CFLAGS) -c htable.c

words.o: words.c words.h tree.h
	$(CC) $(CFLAGS) -c words.c

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c

clean:
	rm -f *.o

