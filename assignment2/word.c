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

void countWords(FILE *file, struct hashTable *table) {
    char *stringBuffer;
    int bufferLength = 256;
    int bufferSize = 0;

    if ((stringBuffer = (char *)malloc(bufferLength * sizeof(*stringBuffer))) == NULL) {
        // Do somehing
    }

    while (1) {
        char c = fgetc(file);
        if (feof(file)) {
            break;
        } else {
            if (bufferSize == bufferLength) {
                bufferLength *= 2;
                if ((stringBuffer = (char *)realloc(stringBuffer-bufferSize, bufferLength))
                        == NULL) {
                    // Do something
                }
                stringBuffer += bufferSize;
            }

            if (isalpha(c) && c != '\0') {
                *stringBuffer = c;
                stringBuffer++;
                bufferSize++;
            } else {
                int i;

                *stringBuffer = '\0';
                stringBuffer -= bufferSize;
                insertHashTable(table, stringBuffer);
                for (i = 0; stringBuffer[i]; i++) {
                    stringBuffer[i] = '\0';
                }
                bufferSize = 0;
            } 
        }
    }
    free(stringBuffer);
}
