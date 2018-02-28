#include "archive.h"

int writeHeader(char *finputName, FILE *archive) {
    struct tarHeader header = {0};
    unsigned char *_header = (unsigned char *)&header;
    struct stat statBuf;
    struct passwd *user = NULL;
    struct group *group = NULL;
    int checksum, i;


    /* TODO: Get name from "path" as provided */
    strcpy(header.name, finputName);

    if (stat(finputName, &statBuf) < 0) {
        perror("Could not stat argv[1]");
        return 0;
    }
    
    sprintf(header.mode, "%07o", statBuf.st_mode);
    header.mode[0] = '0';
    header.mode[1] = '0';
    header.mode[2] = '0';

    sprintf(header.uid, "%07o", statBuf.st_uid);

    sprintf(header.gid, "%07o", statBuf.st_gid);

    sprintf(header.size, "%011lo", statBuf.st_size);

    sprintf(header.mtime, "%011lo", statBuf.st_mtime);

    if (S_ISREG(statBuf.st_mode) > 0) {
        header.typeflag = REGTYPE;
    } else if (S_ISLNK(statBuf.st_mode) > 0) {
        header.typeflag = SYMTYPE;
    } else if (S_ISDIR(statBuf.st_mode) > 0) {
        header.typeflag = DIRTYPE;
    }

    /* TODO: Do link name */

    strcpy(header.magic, TMAGIC);

    strcpy(header.version, TVERSION);

    user = getpwuid(statBuf.st_uid);
    strcpy(header.uname, user->pw_name);
    
    group = getgrgid(statBuf.st_gid);
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
    memset(readBuf, 0, TBLOCKSIZE);
    fwrite(readBuf, TBLOCKSIZE, 1, archive);
    fwrite(readBuf, TBLOCKSIZE, 1, archive);
    
    fclose(finput);
    return 1;
}
