#include "mush.h"

volatile int childPID = 0;
volatile int interrupted = 0;
static struct sigaction sa;

void pipeline(struct cmd *stages, int stageCount) {
    int readPipe[2]={0}, writePipe[2]={0};
    int stageNum;

    pipe(readPipe);
    for (stageNum = 0; stageNum < stageCount; stageNum++) {
        struct cmd *stage = &stages[stageNum];
        int childStatus, input, output;

        if (!strcmp(stage->argv[0], "cd")) {
            chdir(stage->argv[1]);
            continue;
        }

        pipe(writePipe);
        if ((childPID = fork()) == 0) {              /* Child */
            if (stage->input != NULL) {
                if (stageNum != 0) {
                    fprintf(stderr, "mush: ambiguous input\n");
                    exit(2);
                }
                if ((input = open(stage->input, O_RDONLY)) < 0) {
                    fprintf(stderr, "mush: %s: %s\n", stage->input, strerror(errno));
                    exit(1);
                }
                dup2(input, STDIN_FILENO);
                close(input);
            }
            if (stage->output != NULL) {
                if (stageNum != stageCount - 1) {
                    fprintf(stderr, "mush: ambiguous output\n");
                    exit(2);
                }
                umask(022);
                if ((output = open(stage->output, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
                    fprintf(stderr, "mush: %s: %s\n", stage->output, strerror(errno));
                    exit(1);
                }
                dup2(output, STDOUT_FILENO);
                close(output);
            }
            if (stageCount > 1) {
                if (stageNum == 0) {
                    dup2(writePipe[1], STDOUT_FILENO);
                } else if (stageNum == stageCount - 1) {
                    dup2(readPipe[0], STDIN_FILENO);
                } else {
                    dup2(readPipe[0], STDIN_FILENO);
                    dup2(writePipe[1], STDOUT_FILENO);
                }
            }
            close(readPipe[0]);
            close(readPipe[1]);
            close(writePipe[0]);
            close(writePipe[1]);
            execvp(stage->argv[0], stage->argv);
            perror("execvp() failed!");
        } else {                                /* Parent */
            waitpid(childPID, &childStatus, 0);
            if (childStatus > 0)
                break;
            close(readPipe[0]);
            close(readPipe[1]);
            close(writePipe[1]);
            dup2(writePipe[0], readPipe[0]);
            close(writePipe[0]);
        }
    }
    return;
}

void catchInt(int signum) {
    kill(childPID, SIGINT);
    interrupted = 1;
    /* sigaction(signum, &sa, NULL); */
    return;
}

int sigSafeScan(char *buffer, int bufferLen) {
    char *_buffer = buffer;
    int i = 0;

    while (!interrupted) {
        *_buffer = fgetc(stdin);
        if (i == bufferLen) {
            return i++;
        } else if (feof(stdin)) {
            break;
        } else if (*_buffer == '\n') {
            *_buffer = '\0';
            return i;   
        } else {
            _buffer++;
            i++;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    struct cmd stages[MAXCMDS];
    int cmdBufLen = 0;
    char cmdBuf[CLILEN] = {'\0'}; /* TODO: traling '| '*/
    char *_cmdBuf = (char *)&cmdBuf;
    char cmdBufOrig[CLILEN] = {'\0'};
    char *_cmdBufOrig = (char *)&cmdBufOrig;
    int stageCount=0;

    if (argc > 1) {
        execvp(argv[1], &argv[1]);
        return 0;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = catchInt;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, EINTR);
    sigaction(SIGINT, &sa, NULL);

    while (!feof(stdin)) {
        memset(stages, '\0', MAXCMDS * sizeof(*stages));
        printf(":-P "); 
        cmdBufLen = sigSafeScan(&cmdBuf[0], CLILEN);
        if (cmdBufLen > CLILEN) {
            fprintf(stderr, "command too long\n");
            continue;
        } else if (cmdBufLen == 0) {
            if (feof(stdin)) 
                break;
            if (interrupted)
                interrupted = 0;
            continue;
        }
        strcpy(cmdBufOrig, cmdBuf);
        if ((stageCount = parseLine(stages, _cmdBuf, _cmdBufOrig)) < 0)
            continue;
        pipeline(&stages[0], stageCount);
        memset(cmdBuf, '\0', CLILEN * sizeof(*cmdBuf));
    }

    return 0;
}
