#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void) {

    printf("Hello, world!\n");
    if (fork() == 0) {      /* Child */
        printf("This is the child, pid %i.\n", getpid());
    } else {                /* Parent */
        printf("This is the parent, pid %i.\n", getpid());
        wait(NULL);
        printf("This is the parent, pid %i, signing off.\n", getpid());
    }


    return 0;
}


