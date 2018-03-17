CC = gcc
CFLAGS = -Wall -g -pedantic 
SOURCES = mush.c parseline.c 
HEADERS = mush.h parseline.h
.PHONY: clean

all : $(SOURCES) $(HEADERS) 
	$(CC) $(CFLAGS) -o mush $(SOURCES)

clean:
	rm -f *.o
