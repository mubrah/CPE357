#include "extract.h"

int readHeader(char *finputName, FILE *archive) {
    struct tarHeader header = {0};

    fread(&header, TBLOCKSIZE, 1, archive);

    return 1;
}

int checksum(char *tarBlock) {
    int checksum = 0;
    int i;

    for (i = 0; i < TBLOCKSIZE; i++) {
        checksum += (int)tarBlock[i];
    }

    return checksum;
}

int extractData(char *foutputName, FILE *archive) {
    FILE *foutput;
    char readBuf[TBLOCKSIZE] = {0};
    char emptyBlock[TBLOCKSIZE] = {0};
    int endRecordCount = 0;

    foutput = fopen(foutputName, "wb");
    
    while (fread(readBuf, TBLOCKSIZE, 1, archive)) {
        if (feof(archive)) {
            break;
        } else if (ferror(archive)) {
            printf("Could not read entire block\n");
        } else {
            if (checksum(readBuf) == 0) {
                endRecordCount++;
                if (endRecordCount == 2) {
                    break;
                } else {
                    continue;
                }
            } else {
                if (endRecordCount > 0) {
                    fwrite(emptyBlock, TBLOCKSIZE, 1, foutput);
                }
                fwrite(readBuf, TBLOCKSIZE, 1,  foutput);
                memset(readBuf, 0, TBLOCKSIZE);
                continue;
            }
        }
    }
    fclose(foutput);
    /* TODO: Maybe reopen and delete trailing '\0' from data block padding*/
    return 1; 
}
