#include <stdio.h>
#include <stdlib.h>
#include "pwordcount.h"


//char *read_file(char *file_name);

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