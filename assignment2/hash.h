#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef FW_MAIN_H
#endif

#ifndef WORD_H
#include "word.h"
#endif

#ifndef HASH_H
#define HASH_H

struct hashTable {
    int capacity;
    int count;
    struct chain **buckets;
};

struct chain {
    struct wordCount *word;
    struct chain *next;
};

int hashString(char *str);
struct hashTable *makeHashTable(void);
void destroyHashTable(struct hashTable *table);
int chainContains(struct chain *toSearch, char *string);
struct chain *addChain(struct chain *existingChain, struct wordCount *newWord, int *uniqCount);
void insertHashTable(struct hashTable *table, struct wordCount *word);

#endif  // HASH_H
