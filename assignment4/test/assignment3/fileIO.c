#include "fileIO.h"

/******************************************************************************
 * Encoding IO
 *****************************************************************************/

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
 * Write the bit at a position where the MSB is the LHS represented by a 7
 * and the LSB is the RHS represented by a 0 index. Incomplete bytes will be
 * buffered with 0's to complete the byte.
 */
uint8_t appendWriteBit(int fd, uint8_t buffer, int binVal, int *buffWrIdx) {
    uint8_t shiftVal = binVal;
 
    if (shiftVal == 48) {               /* 0 ASCII = 48 */
        if (*buffWrIdx >= 0) {
            return buffer;
        } else {
            write(fd, &buffer, 1);
            *buffWrIdx = 7;
            return 0;                   /* "Empty" Buffer */
        }
    } else if (shiftVal == 49) {        /* 1 ASCII = 49 */ 
        if (*buffWrIdx >= 0) {
            shiftVal = (int)1;
            shiftVal <<= *buffWrIdx;
            return buffer + shiftVal;
        } else {
            write(fd, &buffer, 1);
            *buffWrIdx = 7;
            return 128;                /* New buffer with 1 in the MSB */
        }
    } else {
        return 0;
    }
}

/******************************************************************************
 * Decoding IO
 *****************************************************************************/

int *readHeader(int fd) {
    int *charFreq = NULL;
    uint32_t numChars;
    uint8_t character;
    uint32_t freq;
    int i;

    read(fd, &numChars, 4);

    if (numChars == 0) {        /* Empty File Scenario */
        if ((charFreq = calloc(1, sizeof(*charFreq))) == NULL) {
            /* Handle NULL */
        }
        *charFreq = -1;
        return charFreq;
    }

    /* Character Frequency Table has indexes:
     * 0-127: ASCII Codes
     * 128  : Number of unique Characters
     * 129  : Total number of chars counted
     */
    if ((charFreq = calloc(NUMCHAR + 2, sizeof(*charFreq))) == NULL) {
        /* Handle NULL */
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