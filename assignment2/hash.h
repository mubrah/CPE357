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
    int uniqCount;
    struct chain **buckets;
};

struct chain {
    int hash;
    struct wordCount *word;
    struct chain *next;
};

int hashString(char *str);
struct hashTable *makeHashTable(void);
void destroyHashTable(struct hashTable *table);
int chainContains(struct chain *toSearch, char *string);
struct chain *addChain(struct chain *existingChain, char *word, int *uniqCount, int hash);
void insertHashTable(struct hashTable *table, char *word);

#endif  // HASH_H
