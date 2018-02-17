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

#ifndef HDECODE_H
#define HDECODE_H

void decodeMessage(int infd, int outfd, struct node *htree);
void cleanup(int infd, int outfd, int *charFreqTable, struct node *htree,
    int mode);
int main(int argc, char **argv);
#endif