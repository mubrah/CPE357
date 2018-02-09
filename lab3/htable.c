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

int main(int argc, char **argv) {
    int numNodes = 0;
    int i = 0;
    char *fileStr = NULL;
    int *charFreqTable = NULL;
    struct node *HTree = NULL;
    struct node **finalCodes;

    FILE *file = fopen(argv[1], "r");
    fileStr = fileToStr(file);
    fclose(file);
    charFreqTable = countChars(fileStr);
    free(fileStr);
    HTree = createHTree(charFreqTable);
    numNodes = charFreqTable[NUMCHAR];
    free(charFreqTable);
    if ((finalCodes = calloc(numNodes, sizeof(*finalCodes))) == NULL) {
        /* Handle NULL */
    } 
    getCodes(HTree, finalCodes, NULL);
    sortHNodeChar(finalCodes, numNodes);
    for (i = 0; i < numNodes; i++) {
        printf("0x%02x: %s\n", (char)finalCodes[i]->string[0], finalCodes[i]->code);
    }

    freeNode(HTree);
    free(finalCodes);
    return 0;
}