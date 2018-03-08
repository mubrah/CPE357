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

void parseCommand(struct cmd *stage, char **_cmdBuf, char **_cmdBufOrig,
        int *stageNum, int nextPipeOffset) {
    char *token=NULL;
    char argDlm[2] = " \0";
    char *input=NULL, *output=NULL;
    int _argc = 0;

    token = strtok(*_cmdBuf, argDlm);
    while ((token != NULL) && (token < *_cmdBuf + nextPipeOffset)) {
        if (*stageNum >= MAXCMDS) {
            fprintf(stderr, "pipeline too deep\n");
            exit(1);
        }

        if (!strcmp(token, "<")) {
            if (input != NULL) {
                fprintf(stderr, "bad input redirection\n");
                exit(1);
            }
            token = strtok(NULL, argDlm);
            input = token;
            token = strtok(NULL, argDlm);
        } else if (!strcmp(token, ">")) {
            if (output != NULL) {
                fprintf(stderr, "bad input redirection\n");
                exit(1);                    
            }
            token = strtok(NULL, argDlm);
            output = token;
            token = strtok(NULL, argDlm);
        } else {
            if (!strcmp(token, "|")) {
                fprintf(stderr, "invalid null command\n");
                exit(1);
            }
            if (_argc >= MAXARGS) {
                fprintf(stderr, "too nany arguments\n");
                exit(1);
            }
            stage->argv[_argc] = token;
            token = strtok(NULL, argDlm);
            _argc++;
        }
    }

    stage->cmd = *_cmdBufOrig;
    stage->cmdLen = nextPipeOffset;
    stage->input = input;
    stage->output = output;
    stage->argc = _argc;

    *_cmdBuf += nextPipeOffset + 2;
    *_cmdBufOrig += nextPipeOffset + 3,
    (*stageNum)++;
}

int parseLine(struct cmd *stages, char *cmdBuf, char *_cmdBuf,
        char *cmdBufOrig, char *_cmdBufOrig) {
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

        parseCommand(&stages[stageNum], &_cmdBuf, &_cmdBufOrig, &stageNum,
            nextPipeOffset);

        if (lastOperation) {
            break;
        }
    }
    return stageNum;
}

void printParsedCommands(struct cmd *stage, int _stageNum, int lastOperation) {
    int i  = 0;
    
    printf("\n");
    printf("--------\n");
    if (_stageNum) {
        printf("Stage %i: \" ", _stageNum);    
    } else {
        printf("Stage %i: \"", _stageNum);
    }
    fwrite(stage->cmd, sizeof(char), stage->cmdLen, stdout);
    if (lastOperation) {
        printf(" \"\n");
    } else {
        printf("\"\n");
    }
    printf("--------\n");
    printf("%10s: %s\n", "input", stage->input);
    printf("%10s: %s\n", "output", stage->output);
    printf("%10s: %i\n", "argc", stage->argc);
    printf("%10s: ", "argv");
    for (i = 0; i < stage->argc - 1; i++) {
        printf("\"%s\",", stage->argv[i]);
    }
    printf("\"%s\"\n", stage->argv[stage->argc - 1]);
}

void dumpParsedCommands(struct cmd *stage, int stageNum, int _stageNum) {
    char inMsg[256] = {'\0'};
    char outMsg[256] = {'\0'};
    
    if (_stageNum) {
        if (stage->input != NULL) {
            /* Need to check this outside of loop */
            fprintf(stderr, "ambiguous input\n");
            exit(1);
        }
        sprintf(inMsg, "%s %i", "pipe from stage", _stageNum - 1);
        stage->input = inMsg;
    } else {
        if (stage->input == NULL) {
            strcpy(inMsg, "original stdin");
            stage->input = inMsg;
        }
    }

    if (_stageNum != stageNum - 1) {
        if (stage->output != NULL) {
            fprintf(stderr, "ambiguous output\n");
            exit(1);
        }
        sprintf(outMsg, "%s %i", "pipe to stage", _stageNum + 1);
        stage->output = outMsg;
    } else {
        if (stage->output == NULL) {
            strcpy(outMsg, "original stdout");
            stage->output = outMsg;
        }
    }
    printParsedCommands(stage, _stageNum, stageNum - 1 - _stageNum);
}

int main(int argc, char **argv) {
    struct cmd stages[MAXCMDS];
    char cmdBuf[CLILEN] = "a | b | c";/*{'\0'};*/ /* TODO: traling '| '*/
    char *_cmdBuf = (char *)&cmdBuf;
    char cmdBufOrig[CLILEN] = {'\0'};
    char *_cmdBufOrig = (char *)&cmdBufOrig;
    int stageNum=0, _stageNum = 0;

    memset(stages, '\0', MAXCMDS * sizeof(*stages));

    
    printf("line: ");
    /*scanf("%[^\n\r]", cmdBuf);*/
    if (strlen(cmdBuf) > CLILEN) {
        fprintf(stderr, "command too long\n");
        exit(1);
    } else if (strlen(cmdBuf) == 0) {
        fprintf(stderr, "invalid null command\n");
        exit(1);
    }
    strcpy(cmdBufOrig, cmdBuf);

    stageNum = parseLine(stages, cmdBuf, _cmdBuf, cmdBufOrig, _cmdBufOrig);

    for (_stageNum = 0; _stageNum < stageNum; _stageNum++) {
        struct cmd *stage = &stages[_stageNum];
        
        dumpParsedCommands(stage, stageNum, _stageNum);
    }

    return 0;
}
