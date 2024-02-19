#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define BLOCKSIZE 512
#define MEMSIZE 4096
#define HEADERSIZE 8

unsigned char *arr[BLOCKSIZE/2];

// Standard malloc and free test on 1 huge block.
void standard_malloc_free() {
    // Malloc it
    char* pointer = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE)-1);
    printf("the pointer address %llu\n", &pointer);
    // Free it
    free(pointer);
    free(pointer);
    // Malloc again
    char* pointer2 = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE)-1);
   /* for(int i = 0; i < BLOCKSIZE; i++) {
        *pointer2 = i%256;
        printf("%d", *pointer2);
    }
    printf("/n"); */
}

// Malloc Error 1 (Too large of a Block)
void malloc_huge() {
    char* pointer = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE));
}

// Malloc 1 less than size of array and try to malloc 1 more to see if it works.
void malloc_huge_minus_one() {
    char *pointer = malloc(sizeof(char)* (MEMSIZE-HEADERSIZE-1));
    char *pointer2 = malloc(sizeof(char));
}

// Malloc an entire array of chars and count to make sure the all the spaces have been used
// All the bytes for headers and payload should add up to the MEMSIZE
int malloc_max_number() {
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

// Free 2 adjacent memory addresses to see if coalesce will work.
int free_coalesce_adj_test() {
    malloc_max_number();
    // Free up only half of the
    for(int i = 0; i < BLOCKSIZE/2; i+=2) {
        free(arr[i]);
    } 


    for(int i = 1; i < BLOCKSIZE/2; i+=2) {
        free(arr[i]);
    }  
    char* new_ptr = malloc(sizeof(char) * ((MEMSIZE-HEADERSIZE-1)));
    return 1;
    /*double *array_pointers[BLOCKSIZE/4];
    for(int i = 0; i < BLOCKSIZE/4; i++) {
        array_pointers[i] = malloc(sizeof(double) *2);
        *array_pointers[i] = i + 0.25;
        *(array_pointers[i]+1) = i + 0.5;
    }
    int errors = 0;
    for(int i = 0; i < BLOCKSIZE/4; i++) {
        if(*array_pointers[i] != i + 0.25) errors++;
        if(*(array_pointers[i]+1) != i + 0.5) errors++;
    }
    return errors;*/
}

// Speed test for malloc() and free() as well as the combination of them.
struct timeval malloc_start, malloc_stop;

void speed_test(char choice) {
    if(choice <= 0) {
        gettimeofday(&malloc_start, NULL);
        malloc_max_number();
        gettimeofday(&malloc_stop, NULL);
        printf("took %lf ms\n", ((malloc_stop.tv_sec - malloc_start.tv_sec) * 1000000 + malloc_stop.tv_usec - malloc_start.tv_usec)* 0.001);
    }
    if(choice <= 1) {
        
        // Run code for freeing... and add the same speed test format except for free...
    }
}

int my_coalesce_test() {
    // Malloc it
    char* pointer = malloc((sizeof(char) * (MEMSIZE-HEADERSIZE)-1)/4);
    char* pointer2 = malloc((sizeof(char) * (MEMSIZE-HEADERSIZE)-1)/4);
    char* pointer3 = malloc((sizeof(char) * (MEMSIZE-HEADERSIZE)-1)/4);
    // Free it
    free(pointer);
    free(pointer3);
    free(pointer2);
    // Malloc again
   unsigned char* pointer4 = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE)-1);
    for(int i = 0; i < BLOCKSIZE; i++) {
        *(pointer4+i) = i%256;
    }
    // Check for errors
    int errors = 0;
    for(int i = 0; i < BLOCKSIZE; i++) {
        if(*(pointer4+i) != i%256) errors++;
        
    }
    return errors;
}

int main(int argc, char **argv)
{
//    double* a = malloc(sizeof(double));
//    char* b = malloc(sizeof(char)*7);
   // malloc_max_number();
//    malloc_huge_minus_one();
    // printf("Number of Total Errors: %d\n", malloc_max_number());
    //speed_test(0);
    //standard_malloc_free();
   // printf("Number of Total errors for coalesce: %d\n", my_coalesce_test());
     free_coalesce_adj_test();

    return EXIT_SUCCESS;
}
