CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
OBJECTS = hencode.o htable.o words.o tree.o
.PHONY: clean

main : $(OBJECTS)
	$(CC) $(CFLAGS) -o hencode $(OBJECTS)

hencode.o: hencode.c htable.h
	$(CC) $(CFLAGS) -c hencode.c

htable.o: htable.c htable.h words.h tree.h
	$(CC) $(CFLAGS) -c htable.c

words.o: words.c words.h tree.h
	$(CC) $(CFLAGS) -c words.c

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c

clean:
	rm -f *.o

