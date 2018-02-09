#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef _STRING_H_
#include <string.h>
#endif

#ifndef TREE_H
#include "tree.h"
#endif

#ifndef WORDS_H
#define WORDS_H

#define NUMCHAR 128     /* 128 characters in ASCII */


char *fileToStr(FILE *file);
int *countChars(char *string);
void freeCharFreq(int *charFreq);
struct node *createHTree(int *charFreq);
#endif