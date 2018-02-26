#include "mytar.h"

int main(int argc, char **argv) {
    struct tarHeader header = {0};
    struct stat statBuf;
    struct passwd *user = NULL;
    struct group *group = NULL;
    FILE *archive = NULL;

    /* TODO: Get name from "path" as provided */
    strcpy(header.name, argv[1]);

    if (stat(argv[1], &statBuf) < 0) {
        perror("Could not stat argv[1]");
    }

    
    sprintf(header.mode, "%07o", statBuf.st_mode);
    header.mode[0] = '0';
    header.mode[1] = '0';
    header.mode[2] = '0';

    sprintf(header.uid, "%07o", statBuf.st_uid);

    sprintf(header.gid, "%07o", statBuf.st_gid);

    sprintf(header.size, "%011lo", statBuf.st_size);

    sprintf(header.mtime, "%011lo", statBuf.st_mtime);


    /* TODO: Compute checksum */

    if (S_ISREG(statBuf.st_mode) > 0) {
        header.typeflag = REGTYPE;
    } else if (S_ISLNK(statBuf.st_mode) > 0) {
        header.typeflag = SYMTYPE;
    } else if (S_ISDIR(statBuf.st_mode) > 0) {
        header.typeflag = DIRTYPE;
    }

    /* TODO: Do link name */

    strcpy(header.magic, "ustar\0");

    strcpy(header.version, "00");

    user = getpwuid(statBuf.st_uid);
    strcpy(header.uname, user->pw_name);

    group = getgrgid(statBuf.st_gid);
    strcpy(header.gname, group->gr_name);

    /* Header devmajor and devminor not implemented */

    /* Prefix */


    archive = fopen("test.tar", "wb");
    if (archive != NULL) {
        fwrite(&header, sizeof(struct tarHeader), 1, archive);
        fclose(archive);
    }
    return 0;
}