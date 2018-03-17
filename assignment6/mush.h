#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef _ERRNO_H
#include <errno.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _FCNTL_H
#include <fcntl.h>
#endif

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef _SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifndef _SYS_SIGNAL_H_
#include <signal.h>
#endif

#ifndef PARSELINE_H
#include "parseline.h"
#endif

#ifndef MUSH_H
#define MUSH_H
void pipeline(struct cmd *stages, int stageCount);
int main(int argc, char **argv);

#endif
