/**
 * COMP 7500: Project 2 - Pipe-Based Word Count Tool
 * William Baker
 * Auburn University
 * 
 * test_word_count contains various test cases to verify the correctness of word_count.c.
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "word_count_test.h"

void test_word_count() {
    //Basic tests
    assert(word_count("Hello world") == 2);
    assert(word_count("This is a test") == 4);
    
    //Whitespace
    assert(word_count("  This    is   a test  ") == 4);
    assert(word_count("Line1\nLine2\tLine3\rLine4") == 4);

    //Edge case: Empty string
    assert(word_count("") == 0);
    
    //Edge case: Only spaces
    assert(word_count("       ") == 0);

    //Edge case: Single word
    assert(word_count("Word") == 1);
    
    //Edge case: Multiple words with punctuation
    assert(word_count("Hello... world!! This, again, is a test.") == 7);

    printf("All tests passed!\n");
}

int main() {
    test_word_count();
    return 0;
}