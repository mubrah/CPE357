CC = gcc
CFLAGS = -Wall -g -ansi -pedantic 
.PHONY: clean

forkit : forkit.c
	$(CC) $(CFLAGS) -o forkit forkit.c

tryit : tryit.c
	$(CC) $(CFLAGS) -o tryit tryit.c

clean:
	rm -f *.o *.tar test/*.tar 
