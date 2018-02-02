#include "word.h"

struct wordCount *buildWord(char *str) {
    struct wordCount *newWord;
    if ((newWord = (struct wordCount *)malloc(sizeof(*newWord))) == NULL) {
        /* Handle failure to allocate memory */
    }
    char *_str;
    if ((_str = (char *)malloc(sizeof(*str) * (strlen(str) + 1))) == NULL) {
        /* Handle failure to allocate memory */
    }
    newWord->string = _str;
    strcpy(newWord->string, str);
    newWord->count = 1;
    return newWord;
}

void freeWordCount(struct wordCount *word) {
    free(word->string);
    free(word);
}

struct hashTable *countWords(FILE *file, struct hashTable *table) {
    char *stringBuffer;
    int bufferLength = 256;
    int bufferSize = 0;

    if ((stringBuffer = (char *)malloc(bufferLength * sizeof(*stringBuffer))) == NULL) {
        /* Handle failure to allocate memory */
    }

    while (1) {
        char c = fgetc(file);
        if (feof(file)) {
            break;
        } else {
            if (bufferSize == bufferLength) {
                bufferLength *= 2;
                if ((stringBuffer = (char *)realloc(stringBuffer-bufferSize, bufferLength)) == NULL) {
                    /* Handle failure to allocate memory */
                }
                stringBuffer += bufferSize;
            }

            if (isalpha(c) && c != '\0') {
                *stringBuffer = c;
                stringBuffer++;
                bufferSize++;
            } else {
                if (bufferSize > 0) {
                    int i;

                    *stringBuffer = '\0';
                    stringBuffer -= bufferSize;
                    table = insertHashTable(table, stringBuffer);
                    for (i = 0; stringBuffer[i]; i++) {
                        stringBuffer[i] = '\0';
                    }
                    bufferSize = 0;
                }
            } 
        }
    }
    free(stringBuffer);
    return table;
}
