#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef _STRING_H_
#include <string.h>
#endif

#include <sys/stat.h>

#ifndef _PWD_H_
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

int writeHeader(char *finputName, FILE *archive);
int archiveData(char *finputName, FILE *archive);
#endif
