#include "main.h"

int cmpWordCount(const void *_wc1, const void *_wc2) {
    struct wordCount *wc1 = *(const struct wordCount **)_wc1;
    struct wordCount *wc2 = *(const struct wordCount **)_wc2;
    if (wc1->count == wc2->count) {
        return strcmp(wc2->string, wc1->string);
    } else {
        return wc2->count - wc1->count;
    }
}

int main(int argc, char **argv) {
    int numWords = 10;
    int filenameIdxs[argc-1];
    int fnIdx = 0;
    int idx;
    int debugMode = 0;

    
    for (idx=1; idx < argc; idx++) {
        if (argv[idx][0] == '-') {
            switch (argv[idx][1]) {
                case 'n':
                    if (isdigit(*argv[idx + 1])) {
                        numWords = strtol(argv[idx + 1], NULL, 10);
                        idx++;
                    } else {
                        fprintf(stderr,
                               "usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
                        return 1;
                    }
                    break;
                case 'd':
                    debugMode = 1;
                    break;
                default:
                    fprintf(stderr,
                            "usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
                    return 1;
                    break;
            }
        } else {
            filenameIdxs[fnIdx] = idx;
            fnIdx++;
        }
    }

    struct hashTable *table = NULL;
    if (fnIdx > 0) {
        FILE *file = NULL;
        for (idx = 0; idx < fnIdx; idx++) {
            if ((file = fopen(argv[filenameIdxs[idx]], "r")) == NULL) {
                fprintf(stderr, "%s: No such file or directory\n",
                        argv[filenameIdxs[idx]]);
                continue;
            } else {
                if (table == NULL) {
                    table = makeHashTable();
                }
                table = countWords(file, table);
                fclose(file);
            }
        }
    } else if (fnIdx == 0) {        /* stdin */
        table = makeHashTable();
        table = countWords(stdin, table);
    }
    
    /* Check if all input files were invalid b/c no table was created */
    if (table == NULL) {
        printf("The top %i words (out of 0) are:\n", numWords);
        exit(1);
    }

    int totalWords = 0;
    int _wordCounter = 0;
    struct wordCount *words[table->uniqCount];
    int minWords;
    for (idx = 0; idx < table->capacity; idx++) {
        if (table->buckets[idx] != NULL) {
            struct chain *_buckets = table->buckets[idx];
            while (_buckets != NULL) {
                words[_wordCounter] = _buckets->word;
                totalWords += _buckets->word->count;
                _buckets = _buckets->next;
                _wordCounter++;
            }
        }
    }

    minWords = table->uniqCount < numWords ? table->uniqCount : numWords;
    printf("The top %i words (out of %i) are:\n",
           numWords, table->uniqCount);
    qsort(words, table->uniqCount, sizeof(struct wordCount *), cmpWordCount);
    for (idx = 0; idx < minWords; idx++) {
        printf("%9d %s\n", words[idx]->count, words[idx]->string);
    }
    

    if (debugMode) {
        int i;
        for (i=0; i < table->capacity; i++) {
            if (table->buckets[i] != NULL) {
                struct chain *_buckets = table->buckets[i];
                int j = 0;
                while (_buckets != NULL) {
                    printf("[%i][%i]: %s | %i\n", _buckets->hash, j,
                    _buckets->word->string, _buckets->word->count);
                    _buckets = _buckets->next;
                    j++;
                }
            }
        }
    }

    destroyHashTable(table);


    return 0;
}
