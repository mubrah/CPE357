#include <stdio.h>
#include <string.h>

#define CLILEN     512
#define MAXCMDS     10
#define MAXARGS     10

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
    char cmdBuf[CLILEN] = {'\0'};
    char *_cmdBuf = &cmdBuf;
    char cmdBufOrig[CLILEN] = {'\0'};
    char *_cmdBufOrig = &cmdBufOrig;
    char *token=NULL;
    char argDlm[2] = " \0";
    char stageBuf[CLILEN] = {'\0'};
    int stageNum = 0;
    int ret = 0;

    
    printf("line: ");
    scanf("%[^\n\r]", cmdBuf);
    strcpy(cmdBufOrig, cmdBuf);


    
    while (!ret) {
        int nextPipeOffset=0, redirInOffset=0, redirOutOffset=0;
        char *input=NULL, *output=NULL;
        char origInput[] = "original stdin";
        char origOutput[] = "original stdout";
        char *stageArgs[MAXARGS];
        int argsIdx=0, i=0, lastOperation = 0;


        if ((*_cmdBuf == '|') ||
            (*_cmdBuf == ' ') ||
            (*_cmdBuf == '\0')) {
                _cmdBuf++;
                continue;
        }

        nextPipeOffset = getNextCharOffset(_cmdBuf, " | \0");
        if (nextPipeOffset < 0) {
            nextPipeOffset = strlen(_cmdBuf);
            lastOperation = 1;
        }

        token = strtok(_cmdBuf, argDlm);
        while ((token != NULL) && (token < _cmdBuf + nextPipeOffset)) {
            if (!strcmp(token, "<")) {
                token = strtok(NULL, argDlm);
                input = token;
                token = strtok(NULL, argDlm);
            } else if (!strcmp(token, ">")) {
                token = strtok(NULL, argDlm);
                output = token;
                token = strtok(NULL, argDlm);
            } else {
                stageArgs[argsIdx] = token;
                token = strtok(NULL, argDlm);
                argsIdx++;
            }
        }

        input  = input  ? input  : origInput;
        output = output ? output : origOutput;

        printf("\n");
        printf("--------\n");
        printf("Stage %i: \"", stageNum);
        fwrite(_cmdBufOrig, sizeof(char), nextPipeOffset, stdout);
        printf("\"\n");
        printf("--------\n");
        printf("%10s: %s\n", "input", input);
        printf("%10s: %s\n", "output", output);
        printf("%10s: %i\n", "argc", argsIdx);
        printf("%10s: ", "argv");
        for (i = 0; i < (argsIdx - 1); i++) {
            printf("\"%s\",", stageArgs[i]);
        }
        printf("\"%s\"\n", stageArgs[i]);
        
        memset(stageBuf, '\0', CLILEN);
        _cmdBuf += nextPipeOffset;
        _cmdBufOrig += nextPipeOffset + 3,
        stageNum++;
        if (lastOperation) {
            break;
        }
    }
    return ret;
}
