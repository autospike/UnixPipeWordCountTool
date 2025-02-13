/**
 * COMP 7500: Project 2 - Pipe-Based Word Count Tool
 * William Baker
 * Auburn University
 * 
 * main.c is the "controller" for the pwordcount program.
 * main.c ensures that the user has run the program properly,
 * before it calls the function that forks the program, process_forking.c.
 * 
 * The proper way to run the program is "$ ./pworcount <file_name>"
 * 
 * This program was compiled with the following command:
 * "$ gcc main.c read_file.c process_forking.c word_count.c -o pwordcount -fsanitize=address"
 */

#include <stdio.h>
#include <stdlib.h>
#include "pwordcount.h"

int main(int argc, char *argv[]) {
    printf("Starting main\n");

    //Check the number of arguments to ensure there are not too many or too few
    if (argc != 2) {
        printf("Please enter a file name.\nUsage: ./pwordcount <file_name>\n");
        return -1;
    }
    
    //Read the input file name out of the argument array
    char *file_name = argv[1];

    //Create pipes and fork the process
    process_forking(file_name);

    return 0;
}