#include "words.h"

#define IOBUFFSIZE 1

int *countChars(int fd) {
    int *charFreq = NULL;
    int i = 0;
    unsigned int uniqChar = 0;
    int retRead = 1;
    char readBuffer[IOBUFFSIZE];
    
    /* Character Frequency Table has indexes:
     * 0-127: ASCII Codes
     * 128  : NUmber of unique Characters
     */
    if ((charFreq = calloc(NUMCHAR + 1, sizeof(*charFreq))) == NULL) {
        /* Handle Null */
    }

    while (retRead > 0) {
        retRead = read(fd, readBuffer, IOBUFFSIZE);
        if (charFreq[(int)*readBuffer] == 0) {
            uniqChar++;
        }
        charFreq[(int)*readBuffer]++;
    }
    charFreq[NUMCHAR] = uniqChar;
    return charFreq;            /* MUST Free table */
}

void sortHNodeFreq(struct node **nodeStack, int stackSize) {
    int i = 1;
    int j = 0;

    while (i < stackSize) {
        j = i;
        while (j > 0) {
            if (nodeStack[j - 1]->freq > nodeStack[j]->freq) {
                struct node *temp = nodeStack[j - 1];
                nodeStack[j - 1] = nodeStack[j];
                nodeStack[j] = temp;
            }
            if (nodeStack[j - 1]->freq == nodeStack[j]->freq) {
                if (nodeStack[j - 1]->string[0] > nodeStack[j]->string[0]) {
                    struct node *temp = nodeStack[j - 1];
                    nodeStack[j - 1] = nodeStack[j];
                    nodeStack[j] = temp;
                }
            }
            j--;
        }
        i++;
    }
}

void sortHNodeChar(struct node **nodeList, int stackSize) {
    int i = 1;
    int j = 0;

    while (i < stackSize) {
        j = i;
        while (j > 0) {
            if (nodeList[j - 1]->string[0] > nodeList[j]->string[0]) {
                struct node *temp = nodeList[j - 1];
                nodeList[j - 1] = nodeList[j];
                nodeList[j] = temp;
            }
            j--;
        }
        i++;
    }
}

struct node **createHNodeStack(int *charFreq) {
    int i = 0;
    int stackSize = charFreq[NUMCHAR];
    struct node **HNodeStack;
    struct node **_HNodeStack;

    if ((HNodeStack = malloc(sizeof(*HNodeStack) * stackSize)) != NULL) {
        _HNodeStack = HNodeStack;
    } else {
        /* Handle Null */
    }

    for (i = 0; i < NUMCHAR; i++) {
        if (charFreq[i] > 0) {
            char *string;
            
            if ((string = malloc(sizeof(*string) * 2)) != NULL) {
                string[0] = (char)i;
                string[1] = '\0';
            } else {
                /* Handle Null */ 
            }
            *_HNodeStack = createNode(string, charFreq[i], NULL, NULL, NULL);
            _HNodeStack++;
        }
    }
    sortHNodeFreq(HNodeStack, stackSize);
    return HNodeStack;          /* MUST Free Stack */
}

struct node *createHTree(int *charFreq) {
    int i = 0;
    int j = 1;
    int stackSize = charFreq[NUMCHAR];
    char *string = NULL;
    struct node **HNodeStack = NULL;
    struct node *newNode = NULL;

    HNodeStack = createHNodeStack(charFreq);
    while (HNodeStack[1] != NULL) {
        if ((string = malloc(sizeof(*string) * strlen(HNodeStack[0]->string) + strlen(HNodeStack[1]->string) + 1)) == NULL) {
            /* Handle Null */
        }
        strcpy(string, HNodeStack[0]->string);
        newNode = createNode(strcat(string, HNodeStack[1]->string),
                             HNodeStack[0]->freq + HNodeStack[1]->freq,
                             NULL,
                             HNodeStack[0],
                             HNodeStack[1]);
        HNodeStack[0] = newNode;

        /* Rebuild stack by moving all elements up a position */
        for (i = 1; i < stackSize; i++) {
            if (i == stackSize - 1) {
                HNodeStack[i] = NULL;
            } else {
                HNodeStack[i] = HNodeStack[i + 1];
            }
        }
        sortHNodeFreq(HNodeStack, stackSize - j);
        j++;
    }
    struct node *HTree = HNodeStack[0];
    free(HNodeStack);
    return HTree;       /* MUST Free HTree */
}