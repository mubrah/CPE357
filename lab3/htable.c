#include "htable.h"

void appendString(char *existing, char *toAppend, char *buffer) {
    int lenExisting = 0;
    if (existing != NULL) {
        strcpy(buffer, existing);
        lenExisting = strlen(existing);
    }
    buffer[lenExisting] = *toAppend;
}

void printTree(struct node *HTree, char *bitString) {
    int lenBitString = 0;
    if (bitString != NULL) {
        lenBitString = strlen(bitString);
    }
    if (HTree->left != NULL) {
        char *leftBitString = malloc(sizeof(*leftBitString) * (lenBitString + 1));
        appendString(bitString, "0", leftBitString);
        printTree(HTree->left, leftBitString);
        free(leftBitString);
    } 
    if (HTree->right != NULL) {
        char *rightBitString = malloc(sizeof(*rightBitString) * (lenBitString + 1));
        appendString(bitString, "1", rightBitString);
        printTree(HTree->right, rightBitString);
        free(rightBitString);
    }
    if (HTree->right == NULL && HTree->left == NULL) {
        printf("0x%i: %s\n", (char)HTree->string[0], bitString);
    }
}

int main(int argc, char **argv) {


    FILE *file = fopen(argv[1], "r");
    char *fileStr = fileToStr(file);
    int *charFreq = countChars(fileStr);
    free(fileStr);
    struct node *HTree = createHTree(charFreq);
    free(charFreq);
    printTree(HTree, NULL);
    free(HTree);

    
    return 0;
}