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
int standard_malloc_free() {
    int correct = 1;
    // Malloc it
    int* pointer = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE-1));

    // Checking this
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *(pointer+i) = i;
    }
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        if (*(pointer+i) != i) correct = 0;
    }
    // Free it
    free(pointer);
    // Malloc again
    int* pointer2 = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE-1));
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *(pointer2+i) = i;

    }
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        if (*(pointer2+i) != i) correct = 0;
    }
    free(pointer2);
    return correct;
}

// Malloc Error 1 (Too large of a Block)
int malloc_huge() {
    int* pointer = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE-1));
    int errors = 0;
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *(pointer+i) = i;
    }
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        if (*(pointer+i) != i) errors++;
    }
    return errors;
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

void free_max_number() {
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        free(arr[i]);
    }
}

// Free 2 adjacent memory addresses to see if coalesce will work.
int free_coalesce_adj_test() {
    malloc_max_number();
    // Free up only half of the
    for(int i = 0; i < BLOCKSIZE/2; i+=2) {
        free(arr[i]);
    } 

    // Free the other half now
    for(int i = 1; i < BLOCKSIZE/2; i+=2) {
        free(arr[i]);
    }
    int correct = 1;
    // They should coalesce and I should be able to allocate another 4088 bytes of data
    char* new_ptr = malloc(sizeof(char) * ((MEMSIZE-HEADERSIZE-1)));
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *(new_ptr+i) = i;

    }
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        if (*(new_ptr + i) != i) correct = 0;
    }
    free_max_number();
    return correct;
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
    int correct = 1;
    for(int i = 0; i < BLOCKSIZE; i++) {
        if(*(pointer4+i) != i%256) correct = 0;
        
    }
    return correct;
}

int main(int argc, char **argv)
{
//    double* a = malloc(sizeof(double));
//    char* b = malloc(sizeof(char)*7);
//   malloc_max_number();
//    malloc_huge_minus_one();
    // printf("Number of Total Errors: %d\n", malloc_max_number());
    //speed_test(0);
    //printf("Number of Total errors for coalesce: %d\n", my_coalesce_test());
    //free_coalesce_adj_test();
    char* test_funcs[] = {"Full Malloc and Free Test", "Speed Testing"};

    int num_of_tests = 2;
    printf("Welcome to the testing environment for mymalloc...\n");
    printf("Please choose from the following test functions:\n");
    for(int i =0; i < num_of_tests; i++) {
        printf("%d: %s\n", i, test_funcs[i]);
    }
    int choice = scanf("Choice: ");
    switch(choice) {
        case(0): {
            printf("Running Standard malloc and free test...");
            if(standard_malloc_free() == 1) {printf("Passed\n");}else{printf("Failed\n");}
            printf("Allocating Maximum Space Possible...");
            printf("%d errors\n", malloc_huge());
            printf("Running Basic Coalescing Test...");
            if(my_coalesce_test()) {printf("Passed\n");}else{ printf("Failed");}
            printf("Running Adjacent Free Test to test for coalescing...");
            if(free_coalesce_adj_test()) { printf("Passed\n"); }else{ printf("Failed\n");}
            break;
        }
    }


    return EXIT_SUCCESS;
}
