#ifndef _UNISTD_H_
#include <unistd.h>
#endif

#ifndef TREE_H
#include "tree.h"
#endif

#ifndef WORDS_H
#include "words.h"
#endif


#ifndef HTABLE_H
#define HTABLE_H

/* HTable struct will contain:
 * - Number of Unique Chars
 * - The top of the sorted alphaneumeric character nodes as a list structure
 *   representative of how they will appear in the encoded binary file header
 * - The top of the tree used to generate the encoding table. Can be used for
 *   debug but is used by freeNode to free all the leaves in the HTree
 */
struct HTable {
    unsigned int uniqChars;
    unsigned int totalChars;
    struct node **charFreqNodes;
    struct node *htree;
};

void appendString(char *existing, char *toAppend, char *buffer);
void getCodes(struct node *HTree, struct node **finalCodes, char *bitString);
void freeHTable(struct HTable *htable);
struct HTable *getHTable(int fd);
#endif

