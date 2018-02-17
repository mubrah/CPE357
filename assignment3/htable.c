#include "htable.h"

#define DEBUG

void appendString(char *existing, char *toAppend, char *buffer) {
    int lenExisting = 0;
    if (existing != NULL) {
        strcpy(buffer, existing);
        lenExisting = strlen(existing);
    }
    buffer[lenExisting] = *toAppend;
    buffer[lenExisting + 1] = '\0'; 
}

/* ****************************************************************************
 * struct node **leafNodes is a storage struct with the # of leafs (# of chars
 * with a count > 0 which is kept track in idx 128 of charFreqTable) so the 
 * codes can be used later.
 * ==== ====
 * char *bitstring is used to pass values thru recursive call.
 * Pass a NULL to this parameter at the top call
 *****************************************************************************/
void getCodes(struct node *htree, struct node **leafNodes, char *bitString) {
    int lenBitString = 0;
    if (htree == NULL) {
        leafNodes = NULL;
        return;
    }
    if (bitString != NULL) {
        lenBitString = strlen(bitString);
    }
    if (htree->left != NULL) {
        char *leftBitString = malloc(sizeof(*leftBitString) *
                                    (lenBitString + 2));
        appendString(bitString, "0", leftBitString);
        getCodes(htree->left, leafNodes, leftBitString);
        free(leftBitString);
    } 
    if (htree->right != NULL) {
        char *rightBitString = malloc(sizeof(*rightBitString) *
                                     (lenBitString + 2));
        appendString(bitString, "1", rightBitString);
        getCodes(htree->right, leafNodes, rightBitString);
        free(rightBitString);
    }
    if (htree->right == NULL && htree->left == NULL) {
        if (leafNodes) {
            char *charCode = NULL;
            struct node **_leafNode = leafNodes;
            if ((charCode = malloc(sizeof(*charCode) * strlen(bitString) + 1))
                == NULL) {
                /* Handle NULL */
            }

            strcpy(charCode, bitString);
            charCode[strlen(bitString)] = '\0';
            htree->code = charCode;
            /* Add Node to leafNodes */
            while (*_leafNode) {
                _leafNode++;
            }
            *_leafNode = htree;
        }
    }
}

void freeHTable(struct HTable *htable) {
    freeNode(htable->htree);
    free(htable->charFreqNodes);
    free(htable);
}

/* Top Level Fn to generate Huffman Encoding Table
 * Takes a file descriptor for the input file
 * Returns the huffman encoding table for that file
 */
struct HTable *getHTable(int fd) {
    unsigned int numNodes = 0;
    unsigned int totalChars = 0;
    int *charFreqTable = NULL;
    struct node *htree = NULL;
    struct node **leafNodes = NULL;
    struct HTable *htable = NULL;

    charFreqTable = countChars(fd);
    close(fd);
    htree = createHTree(charFreqTable);
    numNodes = charFreqTable[NUMCHAR];      /* Will be 1 for empty file */
    totalChars = charFreqTable[NUMCHAR + 1];
    free(charFreqTable);
    if ((leafNodes = calloc(numNodes, sizeof(*leafNodes))) == NULL) {
        /* Handle NULL */
    }
    if (totalChars == 0) {                  /* Empty file scenatio */
        leafNodes[0] = htree;                  /* htree is NULL */
    } else if (numNodes == 1) {             /* 1 char file scenario */
        leafNodes[0] = htree;
    } else {                                /* Everything else */
        getCodes(htree, leafNodes, NULL);
        sortHNodeChar(leafNodes, numNodes);

        #ifdef DEBUG
        #include <stdio.h>
        int i;
        for (i = 0; i < numNodes; i++) {
            printf("0x%02x: %s\n", (char)leafNodes[i]->string[0], leafNodes[i]->code);
        }
        #endif

    }
    
    if ((htable = malloc(sizeof(*htable))) == NULL) {
        /* Handle NULL */
    }
    htable->uniqChars = numNodes;
    htable->totalChars = totalChars;
    htable->charFreqNodes = leafNodes;
    htable->htree = htree;
    return htable;      /* MUST free HTable */
}