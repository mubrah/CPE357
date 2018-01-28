#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef FW_MAIN_H
#endif

#ifndef WORD_H
#define WORD_H

#ifndef HASH_H
#include "hash.h"
#endif

struct wordCount {
    char *string;
    int count;
};

struct wordCount *buildWord(char *str);
void freeWordCount(struct wordCount *word);
void countWords(FILE *file, struct hashTable *table);

#endif  /* WORD_H */