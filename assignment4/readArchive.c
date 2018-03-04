#include "readArchive.h"

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

int extractFile(FILE *archive, struct tarHeader *header, char *foutputName) {
    FILE *foutput;
    char readBuf[TBLOCKSIZE] = {0};
    int expectedSize = convOctalStr(header->size);
    int writtenSize = 0;

    foutput = fopen(foutputName, "wb");
    
    while (fread(readBuf, TBLOCKSIZE, 1, archive)) {
        if (feof(archive)) {
            break;
        } else if (ferror(archive)) {
            fprintf(stderr, "Could not read entire block\n");
        } else {
            if (writtenSize + 512 > expectedSize) {
                break;
            } else {
                fwrite(readBuf, TBLOCKSIZE, 1,  foutput);
                writtenSize += 512;
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
        /* fprintf(stderr, "Could not chmod %s\n", header->name); */
    }
    if (chown(header->name,
        convOctalStr(header->uid),
        convOctalStr(header->gid)) < 0) {
            /* fprintf(stderr, "Could not chown %s\n", header->name); */
    }
    return 1; 
}

int extractDir(FILE *archive, struct tarHeader *header) {
    mkdir(header->name, convOctalStr(header->mode));
    
    if (chown(header->name,
        convOctalStr(header->uid),
        convOctalStr(header->gid)) < 0) {
            /* fprintf(stderr, "Could not chown %s\n", header->name); */
    } 
    return 1;
}

int extractSym(FILE *archive, struct tarHeader *header) {
    symlink(header->linkname, header->name);
    
    chmod(header->name, convOctalStr(header->mode));
    if (chown(header->name,
        convOctalStr(header->uid),
        convOctalStr(header->gid)) < 0) {
            /* fprintf(stderr, "Could not chown %s\n", header->name); */
    } 
    return 1;
}

/* bool verbose */
int extractArchive(int argc, char **argv, int verbose) {
    FILE *archive = NULL;
    struct tarHeader header = {0};
    int ret = 0;
    char name[TNAMESIZE + TPREFIXSIZE] = {'\0'};
    char *_name = name;

    archive = fopen(argv[2], "rb");
    while (readHeader(archive, &header) != 0) {
        memset(name, '\0', TNAMESIZE + TPREFIXSIZE);
        strcpy(_name, header.prefix);
        strcpy(_name + strlen(header.prefix), header.name);
        if (verbose) {
            printf("%s\n", name);
        }
        switch (header.typeflag) {
            case REGTYPE:
                if(!extractFile(archive, &header, name)) {
                    fprintf(stderr, "Error extracting %s\n", name);
                    ret = 1;
                }
                break;
            case SYMTYPE:
                if (!extractSym(archive, &header)) {
                    fprintf(stderr, "Error extracting %s\n", name);
                    ret = 1;                
                }
                break;
            case DIRTYPE:
                if(!extractDir(archive, &header)) {
                    fprintf(stderr, "Error extracting %s\n", name);
                    ret = 1;
                }
                break;
        }
    }
    fclose(archive);
    return ret;
}

int listArchive(int argc, char **argv, int verbose) {
    FILE *archive;
    struct tarHeader header = {0};
    int ret = 0;
    char name[TNAMESIZE + TPREFIXSIZE] = {'\0'};
    char *_name = name;

    archive = fopen(argv[2], "rb");
    while (readHeader(archive, &header) != 0) {
        memset(name, '\0', TNAMESIZE + TPREFIXSIZE);
        strcpy(_name, header.prefix);
        strcpy(_name + strlen(header.prefix), header.name);
        if (verbose) {
            /* Verbose Contents listing follows the following format
             * mode             10 chars
             * uname/gname      17 chars
             * size             8  chars
             * mtime            16 chars
             * name             
             */
            char mode[11] = {'\0'};
            int  perms = convOctalStr(header.mode);
            char owner[17] = {'\0'};
            char *_owner = owner;
            int unameLen = strlen(header.uname);
            int gnameLen = strlen(header.gname);
            time_t mtime = convOctalStr(header.mtime);
            struct tm *time = NULL;
            char mtimeBuf[17] = {'\0'};
            
            switch (header.typeflag) {
                case REGTYPE: 
                    mode[0] = '-';
                    break;
                case SYMTYPE:
                    mode[0] = 'l';
                    break;
                case DIRTYPE:
                    mode[0] = 'd';
                    break;
            }
            if (perms & S_IRUSR)
                mode[1] = 'r';
            else mode[1] = '-';
            if (perms & S_IWUSR)
                mode[2] = 'w';
            else mode[2] = '-';
            if (perms & S_IXUSR)
                mode[3] = 'x';
            else mode[3] = '-';
            if (perms & S_IRGRP)
                mode[4] = 'r';
            else mode[4] = '-';
            if (perms & S_IWGRP)
                mode[5] = 'w';
            else mode[5] = '-';
            if (perms & S_IXGRP)
                mode[6] = 'x';
            else mode[6] = '-';
            if (perms & S_IROTH)
                mode[7] = 'r';
            else mode[7] = '-';
            if (perms & S_IWOTH)
                mode[8] = 'w';
            else mode[8] = '-';
            if (perms & S_IXOTH)
                mode[9] = 'x';
            else mode[9] = '-';

            if (unameLen < 17) {
                strncpy(owner, header.uname, unameLen);
                _owner += unameLen;
                *_owner = '/';
                _owner++;
                if (unameLen + gnameLen < 17) {
                    strncpy(_owner, header.gname, gnameLen);
                } else {
                    strncpy(_owner, header.gname, 17 - unameLen);
                }
            } else {
                strncpy(owner, header.uname, 17);
            }

            time = localtime(&mtime);
            sprintf(mtimeBuf, "%04i-%02i-%02i %02i:%02i",
                1900 + time->tm_year,
                time->tm_mon + 1,
                time->tm_mday,
                time->tm_hour,
                time->tm_min);

            printf("%10s %-17s %8i %16s %s\n",
                mode,
                owner,
                convOctalStr(header.size),
                mtimeBuf,
                name);
        } else {
            printf("%s\n", name);
        }
        if (header.typeflag == REGTYPE) {
            if(!extractFile(archive, &header, "/dev/null")) {
                fprintf(stderr, "Error extracting %s\n", name);
                ret = 1;
            }
        }
    }
    fclose(archive);
    return ret;   
}