#include "parseline.h"

/* Takes null-terminated delimiter. Returns -1 if no delimiters found */
int getNextCharOffset(char *current, char *delimiter) {
    int delimLen = strlen(delimiter);
    int offset = 0;
    
    while (*current) {
        if (*current == delimiter[0]) {
            if (!strncmp(current, delimiter, delimLen)) {
                return offset;
            } 
        }
        current++;
        offset++;
    }
    return -1;
}

int parseCommand(struct cmd *stage, char **_cmdBuf, char **_cmdBufOrig,
        int *stageNum, int nextPipeOffset) {
    char *token=NULL;
    char argDlm[2] = " \0";
    char *input=NULL, *output=NULL;
    int _argc = 0;

    token = strtok(*_cmdBuf, argDlm);
    while ((token != NULL) && (token < *_cmdBuf + nextPipeOffset)) {
        if (*stageNum >= MAXCMDS) {
            fprintf(stderr, "pipeline too deep\n");
            return 0;
        }

        if (!strcmp(token, "<")) {
            if (input != NULL) {
                fprintf(stderr, "bad input redirection\n");
                return 0;
            }
            token = strtok(NULL, argDlm);
            input = token;
            token = strtok(NULL, argDlm);
        } else if (!strcmp(token, ">")) {
            if (output != NULL) {
                fprintf(stderr, "bad input redirection\n");
                return 0;                    
            }
            token = strtok(NULL, argDlm);
            output = token;
            token = strtok(NULL, argDlm);
        } else {
            if (!strcmp(token, "|")) {
                fprintf(stderr, "invalid null command\n");
                return 0;
            }
            if (_argc >= MAXARGS) {
                fprintf(stderr, "too many arguments\n");
                return 0;;
            }
            stage->argv[_argc] = token;
            token = strtok(NULL, argDlm);
            _argc++;
        }
    }

    stage->input = input;
    stage->output = output;
    stage->argc = _argc;

    *_cmdBuf += nextPipeOffset + 2;
    *_cmdBufOrig += nextPipeOffset + 3,
    (*stageNum)++;
    return 1;
}

int parseLine(struct cmd *stages, char *_cmdBuf, char *_cmdBufOrig) {
    int stageNum = 0;

    while (1) {
        int nextPipeOffset=0, lastOperation=0;

        nextPipeOffset = getNextCharOffset(_cmdBuf, " | \0");
        if (nextPipeOffset < 0) {
            nextPipeOffset = strlen(_cmdBuf);
            lastOperation = 1;
        }

        if ((*_cmdBuf == ' ') ||
            (*_cmdBuf == '\0')) {
                _cmdBuf++;
                continue;
        }

        if (!parseCommand(&stages[stageNum], &_cmdBuf, &_cmdBufOrig, &stageNum,
                nextPipeOffset))
            return -1;

        if (lastOperation) {
            break;
        }
    }
    return stageNum;
}

