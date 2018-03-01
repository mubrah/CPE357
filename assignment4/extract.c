#include "extract.h"

int readHeader(char *finputName, FILE *archive, struct tarHeader *header) {
    fread(header, TBLOCKSIZE, 1, archive);
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

int octalStr2Int(char *octalString) {
    int res = 0;
    int len = strlen(octalString);
    int i;

    for (i = len - 1; i >= 0; i--) {
        res += (int)((octalString[i] - '0') * pow(8.0, (double)(len - i - 1)));
    }

    return res;
}

int extractFile(char *foutputName, FILE *archive, struct tarHeader *header) {
    FILE *foutput;
    char readBuf[TBLOCKSIZE] = {0};
    int expectedSize = octalStr2Int(header->size);
    int writtenSize = 0;

    foutput = fopen(foutputName, "wb");
    
    while (fread(readBuf, TBLOCKSIZE, 1, archive)) {
        if (feof(archive)) {
            break;
        } else if (ferror(archive)) {
            printf("Could not read entire block\n");
        } else {
            if (writtenSize + 512 > expectedSize) {
                break;
            } else {
                fwrite(readBuf, TBLOCKSIZE, 1,  foutput);
                memset(readBuf, 0, TBLOCKSIZE);
                continue;
            }
        }
    }
    fwrite(readBuf, expectedSize - writtenSize, 1, foutput);
    memset(readBuf, 0, TBLOCKSIZE);
    /* Advance the tar 2 data blocks and close */
    fread(readBuf, TBLOCKSIZE, 1, archive);
    fread(readBuf, TBLOCKSIZE, 1, archive);
    fclose(foutput);

    /* Set up file modes, owners, etc */ 
    int mode = octalStr2Int(header->mode);
    if (chmod(foutputName, mode) < 0) {
        /* ferror("Could not chmod file\n"); */
    }
    int uid = octalStr2Int(header->uid);
    int gid = octalStr2Int(header->gid);
    if (chown(foutputName, uid, gid) < 0) {
        /* ferror("Could not chown file\n"); */ 
    } 

    return 1; 
}

int extractData(char *foutputName, FILE *archive, struct tarHeader *header) {
    if (header->typeflag == '0') {
        return extractFile(foutputName, archive, header);
    }
    return 0;
}