#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef PARSELINE_H
#define PARSELINE_H

#define CLILEN     512
#define MAXCMDS     10
#define MAXARGS     10

struct cmd {
    int argc;
    char *argv[MAXARGS];
    char *input;
    char *output;
};

int getNextCharOffset(char *current, char *delimiter);
int parseCommand(struct cmd *stage, char **_cmdBuf, char **_cmdBufOrig,
        int *stageNum, int nextPipeOffset);
int parseLine(struct cmd *stages, char *_cmdBuf, char *_cmdBufOrig);

#endif
