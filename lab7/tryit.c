#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv) {
    int childStatus, pid;

    if (argc < 2) {
        printf("usage: tryit command\n");
        return 1;
    }
    if ((pid = fork()) == 0) {      /* Child */
        execl(argv[1], argv[1], NULL);
    } else {                /* Parent */
        waitpid(pid, &childStatus, 0);
        if (childStatus != 0) {
            printf("Process %i exited with an error value.\n", pid);
            return 1;
        } else {
            printf("Process %i succeeded.\n", pid);
        }


    }

    return 0;
}
