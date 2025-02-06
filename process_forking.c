#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "pwordcount.h"

#define BUFFER_SIZE 4096

int process_forking(char *file_name) {
    
    //Create pipe (parent --> child)
    int pipe1[2];
    //Create pipe (child --> parent)
    int pipe2[2];
    
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

        //Write from buffer to child
        printf("Process 1 starts sending data to Process 2 ...\n");
        write(pipe1[1], buffer, strlen(buffer) + 1);
        close(pipe1[1]);

        //Wait for child to finish
        wait(NULL);

        //Read and print results from child
        int word_count;
        read(pipe2[0], &word_count, sizeof(word_count));
        close(pipe2[0]);
        printf("Process 1: The total number of words is %d.\n", word_count);

    }
    //Child process
    else if (pid == 0) {
        //Close unused write end of parent --> child
        close(pipe1[1]);
        //Close unused read end of child --> parent
        close(pipe2[0]);
        
        //Read data from parent and save it to a buffer
        char recieved_buffer[BUFFER_SIZE];
        read(pipe1[0], recieved_buffer, sizeof(recieved_buffer));
        close(pipe1[0]);
        printf("Process 2 finishes receving data from Process 1 ...\n");
        
        //INCORRECT COUNTING
        //Count number of words
        printf("Process 2 is counting words now ...\n");
        int count = 0;
        char *token = strtok(recieved_buffer, " \n\t.,!?;:()[]{}");
        while (token != NULL) {
            count++;
            token = strtok(NULL, " \n\t.,!?;:()[]{}");
        }

        //Send results back to parent
        printf("Process 2 is sending the result back to Process 1 ...\n");
        write(pipe2[1], &count, sizeof(count));
        close(pipe2[1]);

        _exit(0); //child process ends and does not continue back to main
    }
    //Fork failed
    else {
        printf("fork{} failed!\n");
        return 1;
    }

    return 0;
}