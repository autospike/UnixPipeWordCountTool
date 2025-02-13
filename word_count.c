/**
 * COMP 7500: Project 2 - Pipe-Based Word Count Tool
 * William Baker
 * Auburn University
 * 
 * word_count receives a buffer of words sent by the parent in process_forking.c.
 * It then counts the words by detecting white space between characters.
 * word_count returns an integer to process_forking.c when done.
 */

#include <ctype.h>
#include "pwordcount.h"

int word_count(const char *buffer) {
    int count = 0;
        int in_word = 0;

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isspace(buffer[i])) {
                in_word = 0;
            }
            else if (in_word == 0) {
                in_word = 1;
                count++;
            }
        }
    return count;
}