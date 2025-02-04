#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "pwordcount.h"

int process_forking(void) {
    int counter = 0;
    pid_t pid = fork();

    if (pid == 0) {
        //child process
        int i = 0;
        for (; i < 5; i++) {
            printf("child process: counter=%d\n", counter++);
        }
        _exit(0); //child process ends and does not continue back to main
    }
    else if (pid > 0) {
        //parent process
        int j = 0;
        for (; j < 5; j++) {
            printf("parent process: counter=%d\n", counter++);
        }
    }
    else {
        //fork failed
        printf("fork{} failed!\n");
        return 1;
    }

    return 0;
}