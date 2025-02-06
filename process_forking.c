#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include "pwordcount.h"

#define BUFFER_SIZE 4096

int process_forking(char *file_name) {
    
    //Create pipe (parent --> child)
    int pipe1[2];
    //Create pipe (child --> parent)
    int pipe2[2];
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //Create child process
    pid_t pid = fork();

    //Determine what the parent and child processes will do

    //Parent process
    if (pid > 0) {
        //Close unused read end of parent --> child
        close(pipe1[0]);
        //Close unused write end of child --> parent
        close(pipe2[1]);
        
        //Call read_file() to check for, open, and load input file
        char *buffer = read_file(file_name);
        
        //If there us an error, send kill signal to child
        if (buffer == NULL) {
            int stop_signal = -1;
            write(pipe1[1], &stop_signal, sizeof(int));
            close(pipe1[1]);
            exit(EXIT_FAILURE);
        }

        //Send benign signal so that the child knows to continue
        int stop_signal = 0;
        write(pipe1[1], &stop_signal, sizeof(int));

        printf("Process 1 starts sending data to Process 2 ...\n");
        write(pipe1[1], buffer, strlen(buffer) + 1);
        close(pipe1[1]);

        //Wait for child to finish
        wait(NULL);

        //Read and print results from child
        int word_count;
        read(pipe2[0], &word_count, sizeof(int));
        close(pipe2[0]);
        printf("Process 1: The total number of words is %d.\n", word_count);
    }
    //Child process
    else if (pid == 0) {
        //Close unused write end of parent --> child
        close(pipe1[1]);
        //Close unused read end of child --> parent
        close(pipe2[0]);
        
        //Recieve signal from parent, if there is an error, exit
        int stop_signal;
        read(pipe1[0], &stop_signal, sizeof(stop_signal));

        if (stop_signal == -1) {
            exit(EXIT_FAILURE);
        }

        //Read data from parent and save it to a buffer
        char recieved_buffer[BUFFER_SIZE];
        read(pipe1[0], recieved_buffer, BUFFER_SIZE - 1);
        close(pipe1[0]);
        printf("Process 2 finishes receving data from Process 1 ...\n");
        
        //Count number of words
        printf("Process 2 is counting words now ...\n");
        int count = 0;
        int in_word = 0;

        for (int i = 0; recieved_buffer[i] != '\0'; i++) {
            if (isspace(recieved_buffer[i])) {
                in_word = 0;
            }
            else if (in_word == 0) {
                in_word = 1;
                count++;
            }
        }

        //Send results back to parent
        printf("Process 2 is sending the result back to Process 1 ...\n");
        write(pipe2[1], &count, sizeof(count));
        close(pipe2[1]);

        exit(0); //child process ends and does not continue back to main
    }
    //Fork failed
    else {
        printf("Error: fork failed\n");
        return 1;
    }

    return 0;
}