#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLILEN     512
#define MAXCMDS     10
#define MAXARGS     10

struct cmd {
    char *cmd;
    int cmdLen;
    char *input;
    int inputLen;
    char *output;
    int outputLen;
    int argc;
    char *argv[MAXCMDS * MAXARGS];
};

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

/* assert 0th char of command is first char of cmd after ' | ' */
int main(int argc, char **argv) {
    struct cmd stages[MAXCMDS];
    char cmdBuf[CLILEN] = {'\0'};
    char *_cmdBuf = &cmdBuf;
    char cmdBufOrig[CLILEN] = {'\0'};
    char *_cmdBufOrig = &cmdBufOrig;
    char *token=NULL;
    char argDlm[2] = " \0";
    int stageNum=0, _stageNum, ret = 0;


    
    printf("line: ");
    scanf("%[^\n\r]", cmdBuf);
    strcpy(cmdBufOrig, cmdBuf);


    
    while (!ret) {
        int nextPipeOffset=0, lastOperation=0, _argc=0;
        char *input=NULL, *output=NULL;

        nextPipeOffset = getNextCharOffset(_cmdBuf, " | \0");
        if (nextPipeOffset < 0) {
            nextPipeOffset = strlen(_cmdBuf);
            lastOperation = 1;
        }

        if ((*_cmdBuf == '|') ||
            (*_cmdBuf == ' ') ||
            (*_cmdBuf == '\0')) {
                _cmdBuf++;
                continue;
        }

        token = strtok(_cmdBuf, argDlm);
        while ((token != NULL) && (token < _cmdBuf + nextPipeOffset)) {
            if (!strcmp(token, "<")) {
                token = strtok(NULL, argDlm);
                if (input != NULL) {
                    fprintf(stderr, "bad input redirection\n");
                    exit(1);
                }
                input = token;
                token = strtok(NULL, argDlm);
            } else if (!strcmp(token, ">")) {
                token = strtok(NULL, argDlm);
                if (output != NULL) {
                    fprintf(stderr, "bad input redirection\n");
                    exit(1);                    
                }
                output = token;
                token = strtok(NULL, argDlm);
            } else {
                stages[stageNum].argv[_argc] = token;
                token = strtok(NULL, argDlm);
                _argc++;
            }

            if ((token != NULL) && (!strcmp(token, "|"))) {
                fprintf(stderr, "invalud null command\n");
                exit(1);
            }
        }

        stages[stageNum].cmd = _cmdBufOrig;
        stages[stageNum].cmdLen = nextPipeOffset;
        stages[stageNum].input = input;
        stages[stageNum].output = output;
        stages[stageNum].argc = _argc;

        _cmdBuf += nextPipeOffset;
        _cmdBufOrig += nextPipeOffset + 3,
        stageNum++;
        if (lastOperation) {
            break;
        }
    }

    for (_stageNum = 0; _stageNum < stageNum; _stageNum++) {
        char inMsg[2000] = {'\0'};
        char outMsg[2000] = {'\0'};
        int j = 0;
        struct cmd *stage = &stages[_stageNum];
        
        if (_stageNum) {
            if (stage->input != NULL) {
                fprintf(stderr, "ambiguous input\n");
            }
            sprintf(inMsg, "%s %i", "pipe from stage", _stageNum - 1);
            stage->input = inMsg;
        } else {
            if (stage->input == NULL) {
                strcpy(inMsg, "original standard input");
                stage->input = inMsg;
            }
        }

        if (_stageNum != stageNum - 1) {
            sprintf(outMsg, "%s %i", "pipe to stage", _stageNum + 1);
            stage->output = outMsg;
        } else {
            if (stage->output == NULL) {
                strcpy(outMsg, "original standard output");
                stage->output = outMsg;
            }
        }
        


        printf("\n");
        printf("--------\n");
        printf("Stage %i: \"", _stageNum);
        fwrite(stage->cmd, sizeof(char), stage->cmdLen, stdout);
        printf("\"\n");
        printf("--------\n");
        printf("%10s: %s\n", "input", stage->input);
        printf("%10s: %s\n", "output", stage->output);
        printf("%10s: %i\n", "argc", stage->argc);
        printf("%10s: ", "argv");
        for (j = 0; j < stage->argc - 1; j++) {
            printf("\"%s\",", stage->argv[j]);
        }
        printf("\"%s\"\n", stage->argv[stage->argc - 1]);
    }

    return ret;
}
