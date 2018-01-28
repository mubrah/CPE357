#include "hash.h"


/* Using Jenkin's One-At-A-Time Hashing */
int hashString(char *str) {
    int hash, i;

    for (hash = i = 0; i < strlen(str); i++) {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

struct hashTable *makeHashTable(void) {
    struct hashTable *table;
    struct chain **buckets;

    if ((table = (struct hashTable *)malloc(sizeof(*table))) == NULL) {
        /* Handle failure to allocate memory */
    }

    table->capacity = 32767;      /* 2^15 - 1 = 32767 */
    table->count = 0;
    if ((buckets = (struct chain **)malloc((table->capacity) * sizeof(struct chain *))) == NULL) {
        /* Handle failure to allocate memory */
    }
    table->buckets = buckets;
    return table;
}

void freeChain(struct chain *link) {
    if (link != NULL) {
        struct chain *next = link->next;
        freeWordCount(link->word);
        free(link);
        freeChain(next);
    }
}

void destroyHashTable(struct hashTable *table) {
    int i;
    for (i=0; i < table->capacity; i++) {
        if (table->buckets[i] != NULL) {
            freeChain(table->buckets[i]);
        }
    }
    free(table->buckets);
    free(table);
}

/* This will also increment the count for a wordCount if a match is found */
int chainContains(struct chain *toSearch, char *string) {
    if (toSearch == NULL) {
        return 0;
    } else if (!strcmp(toSearch->word->string, string)) {
        toSearch->word->count += 1;
        return 1;
    } else {
        return chainContains(toSearch->next, string);
    }
}

struct chain *addChain(struct chain *existingChain, char *word, int *uniqCount, int hash) {
    if (existingChain == NULL) {
        struct chain *newChain;
        struct wordCount *newWord;
        if ((newChain = (struct chain *)malloc(sizeof(*newChain))) == NULL) {
            /* Handle failure to allocate memory */
        }
        if ((newWord = (struct wordCount*)malloc(sizeof(*newWord))) == NULL) {
            /* Handle failure to allocate memory */
        }
        newWord = buildWord(word);
        newChain->word = newWord;
        newChain->next = NULL;
        newChain->hash = hash;
        *uniqCount += 1;
        return newChain;
    } else {
        if (!chainContains(existingChain, word)) {
            existingChain->next = addChain(existingChain->next, word, uniqCount, hash);
        }
        return existingChain;
    }
}

void insertHashTable(struct hashTable *table, char *word) {    
    unsigned int hash;
    int offset;
    if (table->count / table->capacity > 0.75) {
        /* Rehash */
    }

    hash = hashString(word);
    offset = hash % (table->capacity);
    table->buckets[offset] = addChain(table->buckets[offset], word, &(table->uniqCount), offset);
}