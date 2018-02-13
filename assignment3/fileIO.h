#ifndef _UNISTD_H_
#include <unistd.h>
#endif

#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _STDINT_H_
#include <stdint.h>
#endif

#ifndef _FCNTL_H_
#include <fcntl.h>
#endif

#ifndef HTABLE_H
#include "htable.h"
#endif

#ifndef WORDS_H
#include "words.h"
#endif

/* Encoding IO */
void writeHeader(int fd, struct HTable *htable);
uint8_t appendWriteBit(int fd, uint8_t buffer, int binVal, int *buffWrIdx);

/* Decoding IO */
int *readHeader(int fd);
int readBit(uint8_t byte, unsigned int idx);