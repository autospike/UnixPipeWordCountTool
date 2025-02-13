/**
 * COMP 7500: Project 2 - Pipe-Based Word Count Tool
 * William Baker
 * Auburn University
 * 
 * process_forking forks the program, creating a parent and a child process.
 * It also creates pipes so that the processes can communicate, and it directs the parent and child on what to do.
 * 
 * The parent calls read_file.c to get the contents of the input file.
 * It then sends those contents to the child, and waits for the child to send back a number
 * After recieving the child's response, the parent prints the results.
 * 
 * The child recieves the contents of the file from the parent, then calls word_count.c to count the number of words.
 * The child then sends this information back to the parent.
 */

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
    
    //Ensure pipe was created correctly
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //Create child process (parent pid > 0, child pid = 0, -1 if failed)
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

        //Send benign signal so that child can continue
        int stop_signal = 0;
        write(pipe1[1], &stop_signal, sizeof(int));

        //Send buffer to child so it can count
        printf("Process 1 starts sending data to Process 2 ...\n");
        write(pipe1[1], buffer, strlen(buffer) + 1);
        close(pipe1[1]);
        free(buffer);
        
        //Wait for child to finish counting
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
        
        //Recieve first signal from parent, if there is an error, exit
        int stop_signal;
        read(pipe1[0], &stop_signal, sizeof(stop_signal));
        if (stop_signal == -1) {
            exit(EXIT_FAILURE);
        }

        //Read data from parent and save it to a buffer
        char recieved_buffer[BUFFER_SIZE];
        read(pipe1[0], recieved_buffer, BUFFER_SIZE - 1);
        close(pipe1[0]);
        printf("Process 2 finishes receiving data from Process 1 ...\n");
        
        //Count number of words
        printf("Process 2 is counting words now ...\n");
        
        int count = word_count(recieved_buffer);

        //Send results back to parent
        printf("Process 2 is sending the result back to Process 1 ...\n");
        write(pipe2[1], &count, sizeof(count));
        close(pipe2[1]);

        exit(0); //child process ends and does not continue back to main
    }
    //Fork failed
    else {
        printf("Error: fork failed\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}