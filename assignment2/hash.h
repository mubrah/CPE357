#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef FW_MAIN_H
#endif

#ifndef HASH_H
#define HASH_H

struct hashTable {
    int* hashes;
    int length;
};

#endif  // HASH_H
