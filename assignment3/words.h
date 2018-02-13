#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _UNISTD_H_
#include <unistd.h>
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


int *countChars(int fd);
void sortHNodeFreq(struct node **nodeStack, int stackSize);
void sortHNodeChar(struct node **nodeList, int stackSize);
struct node *createHTree(int *charFreq);
#endif