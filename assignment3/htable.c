#include "htable.h"

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
 * struct node **FinalCodes is a storage struct with the # of leafs (# of chars
 * with a count > 0 which is kept track in idx 128 of charFreqTable) so the 
 * codes can be used later.
 * If passed a NULL, the codes will be printed in prefixed order.
 * ==== ====
 * char *bitstring is used to pass values thru recursive call.
 * Pass a NULL to this parameter at the top call
 *****************************************************************************/
void getCodes(struct node *HTree, struct node **finalCodes, char *bitString) {
    int lenBitString = 0;
    if (bitString != NULL) {
        lenBitString = strlen(bitString);
    }
    if (HTree->left != NULL) {
        char *leftBitString = malloc(sizeof(*leftBitString) * (lenBitString + 2));
        appendString(bitString, "0", leftBitString);
        getCodes(HTree->left, finalCodes, leftBitString);
        free(leftBitString);
    } 
    if (HTree->right != NULL) {
        char *rightBitString = malloc(sizeof(*rightBitString) * (lenBitString + 2));
        appendString(bitString, "1", rightBitString);
        getCodes(HTree->right, finalCodes, rightBitString);
        free(rightBitString);
    }
    if (HTree->right == NULL && HTree->left == NULL) {
        if (finalCodes) {
            char *charCode = NULL;
            struct node **_finalCodes = finalCodes;
            if ((charCode = malloc(sizeof(*charCode) * strlen(bitString) + 1)) == NULL) {
                /* Handle NULL */
            }
            strcpy(charCode, bitString);
            charCode[strlen(bitString)] = '\0';
            HTree->code = charCode;
            while (*_finalCodes) {
                _finalCodes++;
            }
            *_finalCodes = HTree;
        } else {
            printf("0x%02x: %s\n", (char)HTree->string[0], bitString);
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
    struct node **finalCodes = NULL;
    struct HTable *htable = NULL;

    charFreqTable = countChars(fd);
    close(fd);
    htree = createHTree(charFreqTable);
    numNodes = charFreqTable[NUMCHAR];
    totalChars = charFreqTable[NUMCHAR + 1];
    free(charFreqTable);
    if ((finalCodes = calloc(numNodes, sizeof(*finalCodes))) == NULL) {
        /* Handle NULL */
    } 
    getCodes(htree, finalCodes, NULL);
    sortHNodeChar(finalCodes, numNodes);

    /* Debug Output
    int i = 0;
    for (i = 0; i < numNodes; i++) {
        printf("0x%02x: %s\n", (char)finalCodes[i]->string[0], finalCodes[i]->code);
    }
    */
    
    if ((htable = malloc(sizeof(*htable))) == NULL) {
        /* Handle NULL */
    }
    htable->uniqChars = numNodes;
    htable->totalChars = totalChars;
    htable->charFreqNodes = finalCodes;
    htable->htree = htree;
    return htable;      /* MUST free HTable */
}