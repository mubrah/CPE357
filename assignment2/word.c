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
    char *wordBuffer = NULL;
    char *_wordBuffer = NULL;
    int bufferLength = 256;
    int bufferSize = 0;

    if ((wordBuffer = (char *)malloc(bufferLength *
        sizeof(*wordBuffer))) == NULL) {
        /* Handle failure to allocate memory */
    }
    _wordBuffer = wordBuffer;

    while (1) {
        char c = fgetc(file);
        if ('A' < c && c < 'Z') {
            c += 32; 
        }
        if (feof(file)) {
            break;
        } else {
            if (bufferSize == bufferLength) {
                bufferLength *= 2;
                if ((wordBuffer = (char *)realloc(wordBuffer,
                    bufferLength)) == NULL) {
                    /* Handle failure to allocate memory */
                }
            }

            if (isalpha(c)) {
                *_wordBuffer = c;
                _wordBuffer++;
                bufferSize++;
            } else {
                if (bufferSize > 0) {
                    int i;

                    *_wordBuffer = '\0';
                    table = insertHashTable(table, wordBuffer);
                    for (i = 0; i < bufferLength; i++) {
                        _wordBuffer[i] = '\0';
                    }

                    /* Reset buffer write poitner and buffer size */ 
                    bufferSize = 0;
                    _wordBuffer = wordBuffer;
                }
            } 
        }
    }
    free(wordBuffer);
    return table;
}
