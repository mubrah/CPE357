#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#include "htable.h"


void writeHeader(int fd, struct HTable *htable) {
    uint32_t numChars = htable->uniqChars;
    uint8_t  character;
    uint32_t freq;
    int i;
    
    write(fd, &numChars, 4);

    for (i = 0; i < numChars; i++) {
        character = (uint8_t)htable->charFreqNodes[i]->string[0];
        freq = (uint32_t)htable->charFreqNodes[i]->freq;
        write(fd, &character, 1);
        write(fd, &freq, 4);
    }
}

/* This function appends binVal to the end of the buffer. Given the current
 * buffer "length"; if shifting in causes an overflow the shift will write 
 * the contents of the buffer and return a new uint8 to write to, else 
 * return the existing buffer.
 */
uint8_t binAppendWrite(int fd, uint8_t buffer, int binVal, int bufferLength) {
    uint8_t shiftVal = binVal;

    if (shiftVal == 48) {               /* 0 ASCII = 48 */
        if (bufferLength + 1 < 8) {
            return buffer;
        } else {
            write(fd, &buffer, 1);
            return 0;                   /* "Empty" Buffer */
        }
    } else if (shiftVal == 49) {        /* 1 ASCII = 49 */ 
        shiftVal = (int)1;
        shiftVal <<= 7 - bufferLength;
        if (bufferLength + 1 < 8) {
            return buffer + shiftVal;
        } else {
            write(fd, &buffer, 1);
            return 128;                /* New buffer with 1 in the MSB */
        }
    }
}

void writeMsg(int infd, int outfd, struct HTable *htable) {
    int retRead = 1;
    int i, j;
    char readBuffer[1];
    uint8_t writeBuffer = 0;
    int buffWrIdx = 0;

    while (retRead > 0) {
        retRead = read(infd, readBuffer, 1);
        for (i = 0; i < htable->uniqChars; i++) {
            char *code = htable->charFreqNodes[i]->code;
            if (htable->charFreqNodes[i]->string[0] == *readBuffer) { 
                for (j = 0; j < strlen(code); j++) {
                    writeBuffer = binAppendWrite(outfd, writeBuffer, (int)code[j], buffWrIdx);
                    if (buffWrIdx < 7) {
                        buffWrIdx++;
                    } else {
                        buffWrIdx = 0;
                    }
                }
            }
        }
    }
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

    writeHeader(outfd, htable);
    infd = open(argv[1], O_RDONLY);
    writeMsg(infd, outfd, htable);
    if (infd == -1) {
        perror(argv[1]);
        exit(1);
    }
    
    close(outfd);

    freeHTable(htable);
    exit(0);
}
