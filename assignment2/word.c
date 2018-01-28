#include "word.h"

struct wordCount *buildWord(char *str) {     // Static?
    struct wordCount *newWord = (struct wordCount *)malloc(sizeof(*newWord));
    char *test = (char *)malloc(sizeof(*str) * (strlen(str) + 1));
    newWord->string = test;
    strcpy(newWord->string, str);
    newWord->count = 1;
    return newWord;
}

void freeWordCount(struct wordCount *word) {
    free(word->string);
    free(word);
}
