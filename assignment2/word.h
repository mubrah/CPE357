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
} wordCount;

struct wordCount *buildWord(char *str);
void freeWordCount(struct wordCount *word);

#endif  // WORD_H
