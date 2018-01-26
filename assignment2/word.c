#include "word.h"


static struct wordCount buildWord(char *str) {
    struct wordCount *newWord = malloc(sizeof(struct wordCount));
    newWord->string = malloc(sizeof(char) * (strlen(str) + 1));
    newWord->count = 1;
    return *newWord;
}

void getWords(char *fileName, int **hashList) {
    FILE *file = fopen(fileName, "r");
    int mallocSize = 2;
    char *stringBuffer = (char *) malloc(mallocSize * sizeof(*stringBuffer));
    int bufferLength = 0;
    // TODO: CHECK FOR NULL MALLOC

    while (1) {
        char c = fgetc(file);
        if (feof(file)) {
            break;
        } else {
            if (bufferLength == mallocSize) {
                mallocSize *= 2;
                stringBuffer = (char *) realloc(stringBuffer-bufferLength, mallocSize);
                stringBuffer += bufferLength;
            }

            if (isalpha(c) || isdigit(c)) {
                *stringBuffer = c;
                stringBuffer++;
                bufferLength++;
            } else {
                *stringBuffer = '\0';
                buildWord(stringBuffer);
                stringBuffer -= bufferLength;
            } 
        } 
        printf("%c", c);
    }
    free(stringBuffer);
    fclose(file);
}

void freeWordCount(struct wordCount *word) {
    free(word->string);
    free(word);
}
