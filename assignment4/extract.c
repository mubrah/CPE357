#include "extract.h"

int convOctalStr(char *octalString) {
    int res = 0;
    int len = strlen(octalString);
    int i;

    for (i = len - 1; i >= 0; i--) {
        res += (int)((octalString[i] - '0') * pow(8.0, (double)(len - i - 1)));
    }

    return res;
}

int readHeader(FILE *archive, struct tarHeader *header) {
    fread(header, TBLOCKSIZE, 1, archive);
    return convOctalStr(header->chksum);
}

int extractFile(FILE *archive, struct tarHeader *header) {
    FILE *foutput;
    char readBuf[TBLOCKSIZE] = {0};
    int expectedSize = convOctalStr(header->size);
    int writtenSize = 0;

    foutput = fopen(header->name, "wb");
    
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
    fclose(foutput);

    /* Set up file modes, owners, etc */ 
    int mode = convOctalStr(header->mode);
    if (chmod(header->name, mode) < 0) {
        fprintf(stderr, "Could not chmod %s\n", header->name);
    }
    if (chown(header->name,
        convOctalStr(header->uid),
        convOctalStr(header->gid)) < 0) {
            fprintf(stderr, "Could not chown %s\n", header->name);
    } 

    return 1; 
}

int extractDir(FILE *archive, struct tarHeader *header) {
    mkdir(header->name, convOctalStr(header->mode));
    
    if (chown(header->name, convOctalStr(header->uid), convOctalStr(header->gid)) < 0) {
        fprintf(stderr, "Could not chown %s\n", header->name); 
    } 
    return 1;
}

int extractSym(FILE *archive, struct tarHeader *header) {
    symlink(header->linkname, header->name);
    
    chmod(header->name, convOctalStr(header->mode));
    if (chown(header->name, convOctalStr(header->uid), convOctalStr(header->gid)) < 0) {
        fprintf(stderr, "Could not chown %s\n", header->name); 
    } 
    return 1;
}


int extractArchive(int argc, char **argv) {
    FILE *archive;
    struct tarHeader header = {0};
    int ret = 0;

    archive = fopen(argv[2], "rb");

    while (readHeader(archive, &header) != 0) {
        if (header.typeflag == REGTYPE) {
            if(!extractFile(archive, &header)) {
                fprintf(stderr, "Error extracting %s\n", header.name);
                ret = 1;
            }
        } else if (header.typeflag == SYMTYPE) {
            if (!extractSym(archive, &header)) {
                fprintf(stderr, "Error extracting %s\n", header.name);
                ret = 1;                
            }
        } else if (header.typeflag == DIRTYPE) {
            if(!extractDir(archive, &header)) {
                fprintf(stderr, "Error extracting %s\n", header.name);
                ret = 1;
            }
            
        }
    }
        

    fclose(archive);
    return ret;
}