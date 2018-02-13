#include "words.h"

#define IOBUFFSIZE 1

int *countChars(int fd) {
    int *charFreq = NULL;
    unsigned int uniqChar = 0;
    unsigned int totalChar = 0;
    int retRead = 1;
    char readBuffer[IOBUFFSIZE];
    
    /* Character Frequency Table has indexes:
     * 0-127: ASCII Codes
     * 128  : Number of unique Characters
     * 129  : Total number of chars counted
     */
    if ((charFreq = calloc(NUMCHAR + 2, sizeof(*charFreq))) == NULL) {
        /* Handle Null */
    }

    while (retRead > 0) {
        retRead = read(fd, readBuffer, IOBUFFSIZE);
        if (charFreq[(int)*readBuffer] == 0) {
            uniqChar++;
        }
        if (retRead > 0) {
            totalChar++;
            charFreq[(int)*readBuffer]++;
        }
    }
    charFreq[NUMCHAR] = uniqChar;
    charFreq[NUMCHAR + 1] = totalChar;
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
    struct node *htree = NULL;
    struct node **hnodeStack = NULL;
    struct node *newNode = NULL;

    hnodeStack = createHNodeStack(charFreq);
    while (hnodeStack[1] != NULL) {
        if ((string = malloc(sizeof(*string) * strlen(hnodeStack[0]->string) + strlen(hnodeStack[1]->string) + 1)) == NULL) {
            /* Handle Null */
        }
        strcpy(string, hnodeStack[0]->string);
        newNode = createNode(strcat(string, hnodeStack[1]->string),
                             hnodeStack[0]->freq + hnodeStack[1]->freq,
                             NULL,
                             hnodeStack[0],
                             hnodeStack[1]);
        hnodeStack[0] = newNode;

        /* Rebuild stack by moving all elements up a position */
        for (i = 1; i < stackSize; i++) {
            if (i == stackSize - 1) {
                hnodeStack[i] = NULL;
            } else {
                hnodeStack[i] = hnodeStack[i + 1];
            }
        }
        sortHNodeFreq(hnodeStack, stackSize - j);
        j++;
    }
    htree = hnodeStack[0];
    free(hnodeStack);
    return htree;       /* MUST Free htree */
}