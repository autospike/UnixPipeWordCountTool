#include <stdio.h>
#include "main.h"

int main(void) {
    printf("Starting main\n");

    printf("Entering process_forking\n");
    process_forking();
    printf("Returned from process_forking\n");

    printf("Entering from ipc_pipes\n");
    ipc_pipes();
    printf("Returned from ipc_pipes\n");
    
    return 0;
}