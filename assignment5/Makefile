CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
SOURCES = parseline.c 
HEADERS = parseline.h
.PHONY: clean

all : $(SOURCES) $(HEADERS) 
	$(CC) $(CFLAGS) -o parseline $(SOURCES)

clean:
	rm -f *.o
