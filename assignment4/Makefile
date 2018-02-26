CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
HEADERS = mytar.h
SOURCES = mytar.c
.PHONY: clean

mytar : $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o mytar $(SOURCES)

clean:
	rm -f *.o
