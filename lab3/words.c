#include "words.h"

#define NUMCHAR 128     /* 128 characters in ASCII */


char *fileToStr(FILE *file) {
    char *stringBuffer = NULL;
    char *_stringBuffer = NULL;
    int bufferLength = 256;
    int bufferSize = 1;

    if ((stringBuffer = calloc(bufferLength, sizeof(*stringBuffer))) == NULL) {
        /* Handle null */
        printf("Could not calloc\n");
    } else {
        _stringBuffer = stringBuffer;
    }

    while (1) {
        char c = fgetc(file);
        if (bufferSize == bufferLength) {
            bufferLength *= 2;
            if ((stringBuffer = realloc(stringBuffer, bufferLength) == NULL)) {
                /* Handle Null */
            }
            _stringBuffer = stringBuffer + bufferSize;
        }
        if (feof(file)) {
            *_stringBuffer = '\0';
            break;
        } else {
            *_stringBuffer = c;
            _stringBuffer++;
            bufferSize++;
        }
    }
    return stringBuffer;        /* TODO: Free buffer */
}

int *countChars(char *string) {
    int *charFreq = NULL;
    int i = 0;
    int uniqChar = 0;
    
    /* Character Frequency Table has indexes:
     * 0-127: ASCII Codes
     * 128  : NUmber of unique Characters
     */
    if ((charFreq = calloc(NUMCHAR + 1, sizeof(*charFreq))) == NULL) {
        /* Handle Null */
    }

    for (i = 0; i < strlen(string); i++) {
        if (charFreq[(int)string[i]] == 0) {
            uniqChar++;
        }
        charFreq[(int)string[i]]++;
    }
    charFreq[NUMCHAR] = uniqChar;
    return charFreq;
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

            *_HNodeStack = createNode(string, charFreq[i], NULL, NULL);
            _HNodeStack++;
        }
    }
    qsort(HNodeStack, stackSize, sizeof(struct node **), cmpNodes);

    return HNodeStack;
}

struct node *createHTree(int *charFreq) {
    int i = 0;
    int j = 1;
    int stackSize = charFreq[NUMCHAR];
    char *string;
    struct node **HNodeStack;
    struct node *newNode = NULL;

    HNodeStack = createHNodeStack(charFreq);

    while (HNodeStack[1] != NULL) {
        if ((newNode = malloc(sizeof(*newNode))) == NULL) {
            /* Handle Null */
        }
        if ((string = malloc(sizeof(*string) * (strlen(HNodeStack[0]->string) + strlen(HNodeStack[1]->string) + 1))) == NULL) {
            /* Handle Null */
        }
        strcpy(string, HNodeStack[0]->string);
        newNode->string = strcat(string, HNodeStack[1]->string);
        newNode->freq = HNodeStack[0]->freq + HNodeStack[1]->freq;
        newNode->left = HNodeStack[0];
        newNode->right = HNodeStack[1];

        HNodeStack[0] = newNode;
        for (i = 1; i < stackSize; i++) {
            if (i == stackSize - 1) {
                HNodeStack[i] = NULL;
            } else {
                HNodeStack[i] = HNodeStack[i + 1];
            }
        }

        qsort(HNodeStack, stackSize - j, sizeof(*HNodeStack), cmpNodes);
        j++;
    }
    return HNodeStack[0];
}