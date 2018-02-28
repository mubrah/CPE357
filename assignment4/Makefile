CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
HEADERS = mytar.h archive.h extract.h
SOURCES = mytar.c archive.c extract.c
.PHONY: clean

mytar : $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o mytar $(SOURCES)

clean:
	rm -f *.o test.tar output
