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

void createName(char *nameBuffer, char *prefix, char *name) {
    char *_name = nameBuffer;
    int prefixLen = 0;

    memset(nameBuffer, '\0', TNAMESIZE + TPREFIXSIZE + 1);
    if ((prefixLen = strlen(prefix)) >= TPREFIXSIZE) {
        strncpy(_name, prefix, TPREFIXSIZE);
    } else {
        strcpy(_name, prefix);
    }
    if (prefixLen > 0) {
        _name[prefixLen] = '/';
        prefixLen++;
    }
    if (strlen(name) >= TNAMESIZE) {
        strncpy(_name + prefixLen, name, TNAMESIZE);
    } else {
        strcpy(_name + prefixLen, name);
    }
}

int topStrContains(char *base, char *key) {
    int keyLen = strlen(key);
    char _base[keyLen + 1];
    char _key[keyLen + 1];
    int ret;

    memset(_base, '\0', keyLen + 1);
    memset(_key, '\0', keyLen + 1);
    strcpy(_key, key);
    strncpy(_base, base, keyLen);
    ret = strcmp(_base, _key);
    return ret;
}

/* Actual logic of 'x' flag. This extracts files from the archive */
int __extractArchive(char *name, FILE *archive, struct tarHeader *header,
        int verbose) {
    int ret = 0;

    if (verbose) {
        printf("%s\n", name);
    }
    switch (header->typeflag) {
        case REGTYPE:
            if(!extractFile(archive, header, name)) {
                fprintf(stderr, "Error extracting %s\n", name);
                ret = 1;
            }
            break;
        case SYMTYPE:
            if (!extractSym(archive, header)) {
                fprintf(stderr, "Error extracting %s\n", name);
                ret = 1;                
            }
            break;
        case DIRTYPE:
            if(!extractDir(archive, header)) {
                fprintf(stderr, "Error extracting %s\n", name);
                ret = 1;
            }
            break;
    }
    return ret;
}

/* bool verbose */
int extractArchive(int argc, char **argv, int verbose) {
    FILE *archive = NULL;
    struct tarHeader header = {0};
    int ret = 0, i;
    char name[TNAMESIZE + TPREFIXSIZE] = {'\0'};

    archive = fopen(argv[2], "rb");
    while (readHeader(archive, &header) != 0) {
        createName(&name, header.prefix, header.name);

        if (argc > 3) {
            for (i = 3; i < argc; i++) {
                if (topStrContains(&name, argv[i]) == 0) {
                    ret = __extractArchive(&name, archive, &header, verbose);
                } else {
                    if (header.typeflag == REGTYPE) {
                        extractFile(archive, &header, "/dev/null");
                    }
                }
            }
        } else {
            ret = __extractArchive(&name, archive, &header, verbose);
        }
    }
    fclose(archive);
    return ret;
}

/* Actual logic of 't' flag. This prints the "Table of Contents" */
void __listArchive(char *name, struct tarHeader *header, int verbose) {
    if (verbose) {
        /* Verbose Contents listing follows the following format
        * mode             10 chars
        * uname/gname      17 chars
        * size             8  chars
        * mtime            16 chars
        * name             
        */
        char mode[11] = {'\0'};
        int  perms = convOctalStr(header->mode);
        char owner[17] = {'\0'};
        char *_owner = owner;
        int unameLen = strlen(header->uname);
        int gnameLen = strlen(header->gname);
        time_t mtime = convOctalStr(header->mtime);
        struct tm *time = NULL;
        char mtimeBuf[17] = {'\0'};
        
        switch (header->typeflag) {
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
            strncpy(owner, header->uname, unameLen);
            _owner += unameLen;
            *_owner = '/';
            _owner++;
            if (unameLen + gnameLen < 17) {
                strncpy(_owner, header->gname, gnameLen);
            } else {
                strncpy(_owner, header->gname, 17 - unameLen);
            }
        } else {
            strncpy(owner, header->uname, 17);
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
            convOctalStr(header->size),
            mtimeBuf,
            name);
    } else {
        printf("%s\n", name);
    }
}

int listArchive(int argc, char **argv, int verbose) {
    FILE *archive;
    struct tarHeader header = {0};
    int ret = 0, i;
    char name[TNAMESIZE + TPREFIXSIZE] = {'\0'};

    archive = fopen(argv[2], "rb");
    while (readHeader(archive, &header) != 0) {
        createName(&name, header.prefix, header.name);

        if (argc > 3) {
            for (i = 3; i < argc; i++) {
                if (topStrContains(&name, argv[i]) == 0) {
                    __listArchive(&name, &header, verbose);
                }
            }
        } else {
            __listArchive(&name, &header, verbose);
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
