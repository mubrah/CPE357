#include "hash.h"


// Using Jenkin's One-At-A-Time Hashing
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
        // Handle Null
    }

    table->capacity = 7;      // 2^15 - 1 = 32767
    table->count = 0;
    if ((buckets = (struct chain **)malloc((table->capacity) * sizeof(struct chain *))) == NULL) {
        printf("Poop\n");
    }
    table->buckets = buckets;
    return table;
}

void destroyHashTable(struct hashTable *table) {
    // Need method to recursivly free all chains in buckets
    free(table->buckets);
    free(table);
}

// This will also incremnt the count for a wordCount if a match is found
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

struct chain *addChain(struct chain *existingChain, struct wordCount *newWord) {
    if (existingChain == NULL) {
        struct chain *newChain;
        if ((newChain = (struct chain *)malloc(sizeof(*newChain))) == NULL) {
            // Handle Null
        }
        newChain->word = newWord;
        newChain->next = NULL;
        return newChain;
    } else {
        if (!chainContains(existingChain, newWord->string)) {
            existingChain->next = addChain(existingChain->next, newWord);
        }
        return existingChain;
    }
}

void insertHashTable(struct hashTable *table, struct wordCount *word) {    
    if (table->count / table->capacity > 0.75) {
        // Rehash
    }

    unsigned int hash = hashString(word->string);
    int offset = hash % (table->capacity);
    table->buckets[offset] = addChain(table->buckets[offset], word);
    struct chain *test = table->buckets[offset];
}