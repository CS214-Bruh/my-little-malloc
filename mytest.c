#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define BLOCKSIZE 512
#define MEMSIZE 4096
#define HEADERSIZE 8

char* save = void;

// Malloc Error 1 (Too large of a Block)
void malloc_huge() {
    char* pointer = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE));
    save = pointer;
}

// Malloc 1 less than size of array and try to malloc 1 more to see if it works.
void malloc_huge_minus_one() {
    char *pointer = malloc(sizeof(char)* (MEMSIZE-HEADERSIZE-1));
    char *pointer2 = malloc(sizeof(char));
}

// Malloc an entire array of chars and count to make sure the all the spaces have been used
// All the bytes for headers and payload should add up to the MEMSIZE
int malloc_max_number() {
    unsigned char *arr[BLOCKSIZE/2];
    // malloc each and every entry to the array.
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        arr[i] = malloc(sizeof(char));
    }
    // Give each one a unique number
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *arr[i] = i;
    }
    // Check that each one matches.
    int errors = 0;
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        if(i != *arr[i]) errors++;
    }
    return errors;
}

int main(int argc, char **argv)
{
//    double* a = malloc(sizeof(double));
//    char* b = malloc(sizeof(char)*7);
    malloc_huge();
    free(save);
    malloc_huge();
//    malloc_huge_minus_one();
    //printf("Number of Total Errors: %d\n", malloc_max_number());
    return EXIT_SUCCESS;
}
