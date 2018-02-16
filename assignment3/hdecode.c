#include "hdecode.h"

void decodeMessage(int infd, int outfd, struct node *htree) {
    struct node *_htree = htree;
    uint8_t readBuffer = 0;
    uint8_t writeBuffer = 0;
    int retRead = 1;
    int i, turnLR;

    while(retRead) {
        retRead = read(infd, &readBuffer, 1);
        if (retRead > 0) {
            for (i = 7; i >= 0; i--) {
                if ((_htree->left == NULL) && (_htree->right == NULL)) {
                    writeBuffer = _htree->string[0];
                    if (_htree->freq > 0) {
                        write(outfd, &writeBuffer, 1);
                    }
                    _htree->freq--;
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
            /* write(outfd, &writeBuffer, 1);*/
        }
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
    htree = createHTree(charFreqTable);
    decodeMessage(infd, outfd, htree);
    free(charFreqTable);
    freeNode(htree);
    close(infd);
    close(outfd);
    exit(0);
}