#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

#define MS500 500000

static int count = 0;
static int tick = 0;
static int tock = 0;
static int end = 0;

void catchAlarm(int signum) {
    static int halfsec = 0;

    if (halfsec) {
        tick = 0;
        tock = 1;
        halfsec = 0;
        count--;
    } else {
        tick = 1;
        tock = 0;
        halfsec = 1;
    }

    if (!count) {
        end = 1;
    } 
}

void usage() {
    printf("usage: timeit <seconds>\n");
}

int isNumber(char *string) {
    while(1) {
        if (*string == '\0') {
            return 1;
        } else if (('0' <= *string) && (*string <= '9')) {
            string++;
            continue;
        } else {
            return 0;
        }
    }
}

int main(int argc, char **argv) {
    struct timeval interval = {0, MS500};
    struct timeval value = {0, MS500};
    struct itimerval itimer  = {interval, value};
    struct sigaction sa;


    if (argc != 2) {
        usage();
        exit(1);
    }

    if (!isNumber(argv[1])) {
        fprintf(stderr, "%s: malformed time.\n", argv[1]);
        exit(1);
    } else {
        count = atoi(argv[1]);
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = catchAlarm;
    sigaction(SIGALRM, &sa, NULL);

    if (setitimer(ITIMER_REAL, &itimer, NULL) == -1) {
        perror("error calling setitimer()\n");
        exit(1);
    }
    
    for (;;) {
        pause();
        if (tick) {
            printf("Tick...");
            fflush(stdout);
            tick = 0;
            continue;
        } else if (tock) {
            printf("Tock\n");
            fflush(stdout);
            tock = 0;
            if (end) {
                printf("Time's up!\n");
                break;
            }
            
        }
    }
    return 0;



}



