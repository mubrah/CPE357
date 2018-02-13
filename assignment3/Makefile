CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
HENCODE_SOURCES = hencode.c htable.c words.c tree.c
HDECODE_SOURCES = hdecode.c htable.c words.c tree.c
.PHONY: clean

main : $(OBJECTS)
	$(CC) $(CFLAGS) -o hencode $(HENCODE_SOURCES)
	$(CC) $(CFLAGS) -o hdecode $(HDECODE_SOURCES)



clean:
	rm -f *.o