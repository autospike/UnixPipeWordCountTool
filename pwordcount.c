#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "pwordcount.h"

char* read_file(char *file_name);

int main(int argc, char *argv[]) {
    printf("Starting main\n");

    //Check the number of arguments to ensure there are not too many or too few
    if (argc != 2) {
        printf("Please enter a file name.\nUsage: ./pwordcount <file_name>\n");
        return -1;
    }
    
    char *file_name = argv[1];
    char *buffer = read_file(file_name);

    //Create pipes and fork
    //printf("Entering process_forking\n");
    //process_forking();
    //printf("Returned from process_forking\n");
    
    //Do work and return
    

    //Print output

    return 0;
}

//Check for, open, and load input file
char* read_file(char *file_name) {
    FILE *file = fopen(file_name, "r");

    //If the file could not be opened, print out why
    if (file == NULL) {
        //File DNE
        if (errno == ENOENT) {
            printf("Error: The file %s does not exist\n", file_name);
        }
        //Insufficient perms
        else if (errno == EACCES) {
            printf("Error: You do not have sufficient permissions to open %s\n", file_name);
        }
        //Not a text file
        //else if (strchr(file_name, '.') && strcmp(strrchr(file_name, '.'), ".txt") == 0) {
            //printf("Error: %s is not a text file\n", file_name);
        //}
        //Catchall
        else {
            printf("Error: Could not open file %s : %s\n", file_name, strerror(errno));
        }
    }
    //If the file could be opened, read it into the buffer
    else {
        char *buffer = malloc(ftell(file) + 1);
        printf("Yippie :)\n");
        return buffer;
    }
}