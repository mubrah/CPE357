#include "hdecode.h"

void decodeMessage(int infd, int outfd, struct node *htree) {
    struct node *_htree = htree;
    uint8_t readBuffer = 0;
    uint8_t writeBuffer = 0;
    int retRead = 1;
    int i, turnLR;

    /* If htree->code is NULL we know this is a 1 char file since the root is
     * the leaf. We defined this scenario to have a '0' code.
     */
    if ((htree->left == NULL) && (htree->right == NULL) &&
        (htree->code == NULL)) {
            char *charCode = NULL;
            if ((charCode = malloc(sizeof(*charCode) * 2)) == NULL) {
                    /* Handle NULL */
                }
                /* We are defining a 1 node tree to have the '0' code */
                charCode[0] = '0';
                charCode[1] = '\0';
                htree->code = charCode;
        }
    while(retRead) {
        retRead = read(infd, &readBuffer, 1);
        if (retRead > 0) {
            for (i = 7; i >= 0; i--) {
                if ((_htree->left == NULL) && (_htree->right == NULL)) {
                    writeBuffer = _htree->string[0];
                    write(outfd, &writeBuffer, 1);
                    _htree = htree;
                }
                turnLR = readBit(readBuffer, i);
                if (turnLR == 0) {
                    _htree = _htree->left;
                } else if (turnLR == 1) {
                    _htree = _htree->right;
                }
            }
        } else {
            write(outfd, &writeBuffer, 1);
        }
    }
}

void cleanup(int infd, int outfd, int *charFreqTable, struct node *htree,
             int mode) {
    close(infd);
    close(outfd);
    switch (mode) {
        case 0:
            break;
        case 1:
            free(charFreqTable);
            break;
        default:
            free(charFreqTable);
            freeNode(htree);
            break;
    }
}

int main(int argc, char **argv) {
    int infd, outfd;
    int *charFreqTable = NULL;
    struct node *htree = NULL;

    if (argc == 1) {
        perror("usage: hdecode [(infile | -) [outfile]]\n");
    }
    if ((argv[1][0] == '-') && (strlen(argv[1]) == 1)) {
        infd = STDIN_FILENO;
    } else {
        infd = open(argv[1], O_RDONLY);
        if (infd == -1) {
            perror(argv[1]);
            exit(1);
        }
    }
    if (argc == 2) {
        outfd = STDOUT_FILENO;
    }
    if (argc >= 3) {
        umask(022);
        outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (outfd == -1) {
            perror(argv[2]);
            exit(1);
        }
    }
    
    charFreqTable = readHeader(infd);
    if (*charFreqTable == -1) {             /* Empty file scenario */
        cleanup(infd, outfd, charFreqTable, htree, 0);
        exit(0);
    } else if (charFreqTable[NUMCHAR] == 1) {   /* 1 char file Scenario */
        int i, j;

        for (i = 0; i < NUMCHAR; i++) {
            if (charFreqTable[i] > 0) {
                j = charFreqTable[i];
                while (j > 0) { 
                    write(outfd, &i, 1);
                    j--;
                }
            }
        }
        cleanup(infd, outfd, charFreqTable, htree, 1);
        exit(0);
    }
    htree = createHTree(charFreqTable);
    decodeMessage(infd, outfd, htree);
    cleanup(infd, outfd, charFreqTable, htree, charFreqTable[NUMCHAR]);
    exit(0);
}