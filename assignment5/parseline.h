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
    char *cmd;
    int cmdLen;
    char *input;
    char *output;
    int argc;
    char *argv[MAXCMDS * MAXARGS];
};

int getNextCharOffset(char *current, char *delimiter);
void parseCommand(struct cmd *stage, char **_cmdBuf, char **_cmdBufOrig,
        int *stageNum, int nextPipeOffset);
int parseLine(struct cmd *stages, char *cmdBuf, char *_cmdBuf,
    char *cmdBufOrig, char *_cmdBufOrig);
void printParsedCommands(struct cmd *stage, int _stageNum, int lastOperation);
void dumpParsedCommands(struct cmd *stage, int stageNum, int _stageNum);
int main(int argc, char **argv);

#endif