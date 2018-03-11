CC = gcc
CFLAGS = -Wall -g
SOURCES = timeit.c
.PHONY: clean

mytar : $(SOURCES) 
	$(CC) $(CFLAGS) -o timeit $(SOURCES)

clean:
	rm -f *.o
