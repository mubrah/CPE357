CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
HENCODE_SOURCES = hencode.c fileIO.c htable.c words.c tree.c
HDECODE_SOURCES = hdecode.c fileIO.c htable.c words.c tree.c
.PHONY: clean

all : $(HENCODE_SOURCES) $(HDECODE_SOURCES) 
	$(CC) $(CFLAGS) -o hencode $(HENCODE_SOURCES)
	$(CC) $(CFLAGS) -o hdecode $(HDECODE_SOURCES)

clean:
	rm -f *.o
