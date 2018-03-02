#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _PWD_H
#include <pwd.h>
#endif

#ifndef _GRP_H
#include <grp.h>
#endif

#ifndef MYTAR_H
#include "mytar.h"
#endif

#ifndef ARCHIVE_H
#define ARCHIVE_H

int writeHeader(char *finputName, FILE *archive, struct stat *statBuf);
int archiveData(char *finputName, FILE *archive);
int createArchive(int argc, char **argv, int verbose);
#endif
