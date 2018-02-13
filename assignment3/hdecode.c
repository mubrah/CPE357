#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#include "htable.h"

int *readHeader(int fd) {
    int *charFreq = NULL;
    uint32_t numChars;
    uint8_t character;
    uint32_t freq;
    int i;

    read(fd, &numChars, 4);     /* TODO: Handle error when input file is empty */

    /* Character Frequency Table has indexes:
     * 0-127: ASCII Codes
     * 128  : Number of unique Characters
     * 129  : Total number of chars counted
     */
    if ((charFreq = calloc(NUMCHAR + 2, sizeof(*charFreq))) == NULL) {
        /* Handle Null */
    }
    charFreq[NUMCHAR] = numChars;

    for (i = 0; i < numChars; i++) {
        read(fd, &character, 1);
        read(fd, &freq, 4);
        charFreq[character] = freq;
    }
    return charFreq;        /* MUST free charFreq */
}

/* Read the bit at a position where the MSB is the LHS represented by a 7
 * and the LSB is the RHS represented by a 0 index
 */
int readBit(uint8_t byte, unsigned int idx) {
    uint8_t mask = 1 << idx;
    byte &= mask;
    byte >>= idx;
    return (int)byte;
}

void decodeMessage(int infd, int outfd, struct node *htree) {
    struct node *_htree = htree;
    uint8_t readBuffer;
    char writeBuffer;
    int retRead = 1;
    int i, turnLR;

    while(retRead) {
        retRead = read(infd, &readBuffer, 1);
        for (i = 7; i >= 0; i--) {
            if ((_htree->left == NULL) && (_htree->right == NULL)) {
                writeBuffer = _htree->string[0];
                write(outfd, &writeBuffer, 1);
                _htree = htree;
            }
            turnLR = readBit(readBuffer, i);
            if (turnLR == 0) {
                _htree = _htree->left;
            } else if (turnLR == 1) {
                _htree = _htree->right;
            }
        }
    }
}

int main(int argc, char **argv) {
    int infd, outfd;
    unsigned int numNodes;
    int *charFreqTable = NULL;
    struct node *htree = NULL;
    struct node **finalCodes = NULL;
    struct HTable *htable = NULL;

    if (argc == 1) {
        perror("usage: hdecode [(infile | -) [outfile]]\n");
    }
    
    infd = open(argv[1], O_RDONLY);
    if (infd == -1) {
        perror(argv[1]);
        exit(1);
    }

    if (argc == 2) {
        outfd = 1;
    }

    if (argc == 3) {
        if (argv[1] == "-") {
            close(infd);
            infd = 0;
        }
        outfd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND);
        if (outfd == -1) {
            perror(argv[2]);
            exit(1);
        }
    }
    
    charFreqTable = readHeader(infd);
    htree = createHTree(charFreqTable);
    decodeMessage(infd, outfd, htree);

    /* Debug output
    int i = 0;
    for (i = 0; i < htable->uniqChars; i++) {
        printf("0x%02x: %s\n", (char)finalCodes[i]->string[0], finalCodes[i]->code);
    }
    */

    free(charFreqTable);
    freeNode(htree);
    close(infd);
    close(outfd);
}