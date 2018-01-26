#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef FW_MAIN_H
#endif

#ifndef WORD_H
#define WORD_H

struct wordCount {
    char *string;
    int count;
};

static struct wordCount buildWord(char *str);
void getWords(char *fileNamet, int **hashList);
void freeWordCount(struct wordCount *word);

#endif  // WORD_H
