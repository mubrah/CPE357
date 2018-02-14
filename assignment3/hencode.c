#include "hencode.h"

void encodeMessage(int infd, int outfd, struct HTable *htable) {
    int retRead = 1;
    int i, j;
    uint8_t readBuffer = 0;
    uint8_t writeBuffer = 0;
    int *buffWrIdx = NULL;

    if ((buffWrIdx = calloc(1, sizeof(*buffWrIdx))) == NULL) {
        /* Handle NULL */
    }
    *buffWrIdx = 7;

    while (retRead > 0) {
        retRead = read(infd, &readBuffer, 1);
        if (retRead > 0) {
            for (i = 0; i < htable->uniqChars; i++) {
                if (htable->charFreqNodes[i]->string[0] == readBuffer) {
                    char *code = htable->charFreqNodes[i]->code;
                    for (j = 0; j < strlen(code); j++) {
                        writeBuffer = appendWriteBit(outfd,
                                                    writeBuffer,
                                                    (int)code[j],
                                                    buffWrIdx);
                        *buffWrIdx -= 1;
                    }
                    break;
                }
            }
        } else {
            write(outfd, &writeBuffer, 1);
        }
    }
    free(buffWrIdx);
}

int main(int argc, char **argv) {
    int infd, outfd;
    struct HTable *htable = NULL;

    if (argc == 1) {
        perror("usage: ./hencode infile [ outfile ]\n");
        return 1;
    }
    infd = open(argv[1], O_RDONLY);
    if (infd == -1) {
        perror(argv[1]);
        exit(1);
    }
    if (argc == 2) {
        outfd = STDOUT_FILENO;
    } else {
        umask(022);
        outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (outfd == -1) {
            perror(argv[2]);
            exit(1);
        }
    }

    htable = getHTable(infd);
    close(infd);
    writeHeader(outfd, htable);
    infd = open(argv[1], O_RDONLY);

    if (infd == -1) {
        perror(argv[1]);
        exit(1);
    }

    encodeMessage(infd, outfd, htable);
    close(outfd);
    close(infd);
    freeHTable(htable);
    exit(0);
}
