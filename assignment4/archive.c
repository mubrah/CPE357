#include "archive.h"

int writeHeader(char *finputName, FILE *archive, struct stat *statBuf) {
    struct tarHeader header = {0};
    unsigned char *_header = (unsigned char *)&header;
    struct passwd *user = NULL;
    struct group *group = NULL;
    int checksum, i;


    /* TODO: Get name from "path" as provided */
    strcpy(header.name, finputName);
    
    sprintf(header.mode, "%07o", statBuf->st_mode);
    header.mode[0] = '0';
    header.mode[1] = '0';
    header.mode[2] = '0';

    sprintf(header.uid, "%07o", statBuf->st_uid);

    sprintf(header.gid, "%07o", statBuf->st_gid);

    sprintf(header.mtime, "%011lo", statBuf->st_mtime);

    if (S_ISREG(statBuf->st_mode) > 0) {
        header.typeflag = REGTYPE;
        sprintf(header.size, "%011lo", statBuf->st_size);    
    } else if (S_ISLNK(statBuf->st_mode) > 0) {
        header.typeflag = SYMTYPE;
        readlink(finputName, header.linkname, TNAMESIZE);
    } else if (S_ISDIR(statBuf->st_mode) > 0) {
        header.typeflag = DIRTYPE;
    }
    
    strcpy(header.magic, TMAGIC);

    strcpy(header.version, TVERSION);

    user = getpwuid(statBuf->st_uid);
    strcpy(header.uname, user->pw_name);
    
    group = getgrgid(statBuf->st_gid);
    strcpy(header.gname, group->gr_name);

    /* Header devmajor and devminor not implemented */

    /* Prefix */

    for (i = 0; i < sizeof(struct tarHeader); i++) {
        checksum += *_header;
        _header++;
    }
    sprintf(header.chksum, "%07o", checksum);

    if (archive == NULL) {
        return 0;
    } else {
        fwrite(&header, sizeof(struct tarHeader), 1, archive);
    }
    return 1;
}

int archiveData(char *finputName, FILE *archive) {
    FILE *finput;
    char readBuf[TBLOCKSIZE] = {0};

    finput = fopen(finputName, "rb");

    while (fread(readBuf, TBLOCKSIZE, 1, finput)) {
        if (feof(finput)) {
            break;
        } else if (ferror(finput)) {
            printf("Could not read entire block\n");
        } else {
            fwrite(readBuf, TBLOCKSIZE, 1, archive);
            memset(readBuf, 0, TBLOCKSIZE);
            continue;
        }
    }
    fwrite(readBuf, TBLOCKSIZE, 1, archive);
    fclose(finput);
    return 1;
}

char *appendStr(char *base, char *toAppend) {
    char *newStr = NULL;
    char *_newStr = NULL;
    int len = strlen(base) + strlen(toAppend) + 1;

    newStr = calloc(len, sizeof(*newStr));
    _newStr = newStr + strlen(base);
    strcpy(newStr, base);
    strcpy(_newStr, toAppend);
    return newStr;
}   /* Must free new string */

char *fixDirName(char *origDirName) {
    char *fixedDirName = NULL;

    if (origDirName[strlen(origDirName) - 1] != '/') {
        fixedDirName = appendStr(origDirName, "/");
    } else {
        fixedDirName = malloc(sizeof(*fixedDirName) * (strlen(origDirName) + 1));
        strcpy(fixedDirName, origDirName);
    }
    return fixedDirName;
}   /* Must free fixedDirName */

int createArchive(int argc, char **argv) {
    FILE *archive;
    char emptyBlock[TBLOCKSIZE] = {0};
    int i;

    archive = fopen(argv[2], "wb");
    for (i = 3; i < argc; i++) {
        struct stat statBuf;

        lstat(argv[i], &statBuf);
        if (S_ISREG(statBuf.st_mode) > 0) {
            writeHeader(argv[i], archive, &statBuf);
            archiveData(argv[i], archive);
        } else if (S_ISLNK(statBuf.st_mode) > 0) {
            writeHeader(argv[i], archive, &statBuf);
        } else if (S_ISDIR(statBuf.st_mode) > 0) {  
            DIR *dir = NULL;
            struct dirent *dirEntry = NULL;
            struct stat dirstatBuf;
            struct stat curstatBuf;
            char *dirName = NULL;
            char *fileName = NULL;

            dirName = fixDirName(argv[i]);
            writeHeader(dirName, archive, &statBuf);
            
            dir = opendir(dirName);
            stat(".", &curstatBuf);
            stat(dirName, &dirstatBuf);
            while ((dirEntry = readdir(dir))) {
                if ((dirEntry->d_ino != dirstatBuf.st_ino) &&
                    (dirEntry->d_ino != curstatBuf.st_ino)) {
                        fileName = appendStr(dirName, dirEntry->d_name);
                        lstat(fileName, &statBuf);
                        writeHeader(fileName, archive, &statBuf);
                        archiveData(fileName, archive);
                        free(fileName);
                    }
            }
            closedir(dir);
            free(dirName);
        }
    }
    fwrite(emptyBlock, TBLOCKSIZE, 1, archive);
    fwrite(emptyBlock, TBLOCKSIZE, 1, archive);
    fclose(archive);
    return 0;
}