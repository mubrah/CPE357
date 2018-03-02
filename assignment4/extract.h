#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _MATH_H
#include <math.h>
#endif

#ifndef _STRING_H
#include <string.h>
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

#ifndef EXTRACT_H
#define EXTRACT_H

int readHeader(FILE *archive, struct tarHeader *header);
int extractFile(FILE *archive, struct tarHeader *header);
int extractDir(FILE *archive, struct tarHeader *header);
int extractArchive(int argc, char **argv);
#endif