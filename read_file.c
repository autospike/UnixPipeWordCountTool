/**
 * COMP 7500: Project 2 - Pipe-Based Word Count Tool
 * William Baker
 * Auburn University
 * 
 * read_file ensures that the file the user would like to use is valid.
 * The name of the file is recieved from the parent in process_forking.c.
 * It ensure that the file is a text file, that it exists, and that the user has the right permissions to use the file.
 * read_file then write the contents of the file to the buffer, and sends it back to the parent process in process_forking.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//Check for, open, and load input file
char *read_file(char *file_name) {
    //Open file in read mode
    FILE *file = fopen(file_name, "r");

    //Ensure that the file is a text file
    //Check for a "".""
    char *ext = strrchr(file_name, '.');
    //If there is no "." or the remaining chars are not "txt", print error and exit
    if (ext == NULL || strcmp(ext, ".txt") != 0) {
        printf("Error: '%s' is not a text file\n", file_name);
        return NULL;
    }

    //If the file can't be opened
    if (file == NULL) {
        //File dne
        if (errno == ENOENT) {
            printf("Error: The file '%s' does not exist\n", file_name);
            return NULL;;
        }
        //Insufficient perms
        else if (errno == EACCES) {
            printf("Error: You do not have sufficient permissions to open '%s'\n", file_name);
            return NULL;;
        }
        //Catchall
        else {
            printf("Error: Could not open file '%s' : %s\n", file_name, strerror(errno));
            return NULL;
        }
    }
    //If the file can be opened
    else {
        printf("Process 1: Reading file '%s' now ...\n", file_name);

        //Move file pointer to eof
        fseek(file, 0, SEEK_END);
        //Now that we have moved file pointer to the end, save that position as the file size
        int file_size = ftell(file);
        //Return file pointer to the beginning of the file
        rewind(file);
        
        //If file is empty, return NULL so child can be killed
        if (file_size == 0) {
            printf("Error: This file is empty\n");
            return NULL;
        }
        else {
            //Create a buffer the size of the file + 1 (to add null terminator)
            char *buffer = malloc(file_size + 1);
            //Read files contents  into buffer
            size_t bytesRead = fread(buffer,   1, file_size, file);
            //Add null terminator
            buffer[file_size] = '\0';
            fclose(file);
            return buffer;
        }
    }
}