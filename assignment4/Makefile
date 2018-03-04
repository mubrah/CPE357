CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
HEADERS = mytar.h writeArchive.h readArchive.h
SOURCES = mytar.c writeArchive.c readArchive.c
.PHONY: clean

mytar : $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o mytar $(SOURCES) -lm

clean:
	rm -f *.o *.tar test/*.tar 
