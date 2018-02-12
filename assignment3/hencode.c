#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#include "htable.h"


void writeHeader(int fd, struct HTable htable) {

}

int main (int argc, char **argv) {
    int infd, outfd;
    struct HTable *htable = NULL;

    infd = open(argv[1], O_RDONLY);
    if (infd == -1) {
        perror(argv[1]);
        exit(1);
    }

    htable = getHTable(infd);
    close(infd);

    outfd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND);
    if (outfd == -1) {
        perror(argv[2]);
        exit(1);
    }

    write(outfd, &htable->uniqChars, 1);
    close(outfd);

    freeHTable(htable);
    exit(0);
}