#ifndef _STDIO_H_
#include <stdio.h>
#endif

#ifndef _SYS_TYPES_H_
#include <sys/types.h>
#endif

#ifndef _SYS_STAT_H_
#include <sys/stat.h>
#endif

#ifndef FILEIO_H
#include "fileIO.h"
#endif

#ifndef HTABLE_H
#include "htable.h"
#endif

#ifndef HENCODE_H
#define HENCODE_H

void encodeMessage(int infd, int outfd, struct HTable *htable);
int main(int argc, char **argv);
#endif