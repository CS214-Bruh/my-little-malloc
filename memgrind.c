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

#define NUMMALTESTS 4
#define MAX_INT 2147483647

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

// Malloc Error (Too large of a Block)
int malloc_huge() {
    int* pointer = malloc(sizeof(char) * (MEMSIZE-HEADERSIZE-1));
    int errors = 0;
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *(pointer+i) = i;
    }
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        if (*(pointer+i) != i) errors++;
    }
    free(pointer);
    return errors;
}

// Malloc 1 less than size of array and try to malloc 1 more to see if it works.
void malloc_huge_minus_one() {
    char *pointer = malloc(sizeof(char)* (MEMSIZE-HEADERSIZE-1)-8);
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
    unsigned char* new_ptr = malloc(sizeof(char) * ((MEMSIZE-HEADERSIZE-1)));
    for(int i = 0; i < BLOCKSIZE/2; i++) {
        *(new_ptr+i) = i;

    }
    for(int i = 0; i < BLOCKSIZE/2; i++) {
//        printf("%d\n",*(new_ptr + i));
        if (*(new_ptr + i) != i) correct = 0;
    }
    free(new_ptr);
    return correct;
}

// Speed test for malloc() and free() as well as the combination of them.
struct timeval malloc_start, malloc_stop, free_start, free_stop;

void speed_test(int number_of_times) {
    // Just run speed test for malloc
    int i = 0;
    // Keep track of some stats for malloc
    double malloc_avg = 0;
    double malloc_top = 0;
    double malloc_low = MAX_INT;
    // Some for free
    double free_avg = 0;
    double free_top = 0;
    double free_low = MAX_INT;

    while(i < number_of_times) {
        gettimeofday(&malloc_start, NULL);
        malloc_max_number();
        gettimeofday(&malloc_stop, NULL);
        double malloc_time = ((malloc_stop.tv_sec - malloc_start.tv_sec) * 1000000 + malloc_stop.tv_usec - malloc_start.tv_usec)* 0.001;
        if(malloc_time > malloc_top) malloc_top = malloc_time;
        if(malloc_time < malloc_low) malloc_low = malloc_time;
        malloc_avg += malloc_time;
        // printf("Malloc-ing %d objects took %lf ms\n", BLOCKSIZE/2, malloc_time);
        
        // Run code for freeing... and add the same speed test format except for free...
        gettimeofday(&free_start, NULL);
        free_max_number();
        gettimeofday(&free_stop, NULL);
        double free_time = ((free_stop.tv_sec - free_start.tv_sec) * 1000000 + free_stop.tv_usec - free_start.tv_usec)* 0.001;
        if(free_time > free_top) free_top = free_time;
        if(free_time < free_low) free_low = free_time;
        free_avg += free_time;
        //printf("Freeing %d objects took %lf ms\n", BLOCKSIZE/2,free_time);

        i++;
    }
    printf("Malloc Statistics:\n");
    printf("Total Items Malloc-ed: %d\n", (BLOCKSIZE/2)*number_of_times);
    printf("Average Malloc-ing Time per Run of %d objects: %lf\n", BLOCKSIZE/2, malloc_avg/number_of_times);
    printf("Slowest Run of %d objects: %lf\n", BLOCKSIZE/2, malloc_top);
    printf("Fastest Run of %d objects: %lf\n", BLOCKSIZE/2, malloc_low);

    printf("Free Statistics:\n");
    printf("Total Items Freed: %d\n", (BLOCKSIZE/2)*number_of_times);
    printf("Average Freeing Time per Run of %d objects: %lf\n", BLOCKSIZE/2, free_avg/number_of_times);
    printf("Slowest Run of %d objects: %lf\n", BLOCKSIZE/2, free_top);
    printf("Fastest Run of %d objects: %lf\n", BLOCKSIZE/2, free_low);

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
    free(pointer4);
    return correct;
}

void error_test() {
    printf("Running Error Testing...\n");
    printf("Malloc Error Test 1: Malloc Too Much Memory\n");
    malloc(sizeof(char) * MEMSIZE-HEADERSIZE+1);
    printf("Malloc Error Test 2: Malloc More Than Available Memory\n");
    char* ptr = malloc(sizeof(char) * MEMSIZE-HEADERSIZE-1);
    char* ptr2 = malloc(sizeof(char));
    printf("Free Error Test 1: \n");
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
    char* test_funcs[] = {"Full Malloc and Free Test", "Speed Testing", "Error Testing"};

    int num_of_tests = 3;
    printf("Welcome to the testing environment for mymalloc...\n");
    printf("Please choose from the following test functions:\n");
    for(int i =0; i < num_of_tests; i++) {
        printf("%d: %s\n", i+1, test_funcs[i]);
    }
    int choice;
    scanf("%d", &choice);
    switch(choice) {
        case(1): {
            int passed_subtests = 0;
            printf("Running Standard malloc and free test...");
            if(standard_malloc_free() == 1) {printf("Passed\n");passed_subtests++;}else{printf("Failed\n");}
            printf("Allocating Maximum Space Possible...");
            int mal_huge_errors = malloc_huge();
            printf("%d errors\n", mal_huge_errors);
            if(mal_huge_errors == 0) passed_subtests++;
            printf("Running Basic Coalescing Test...");
            if(my_coalesce_test()) {printf("Passed\n");passed_subtests++;}else{ printf("Failed");}
            printf("Running Adjacent Free Test to test for coalescing...");
            if(free_coalesce_adj_test()) { printf("Passed\n"); passed_subtests++;}else{ printf("Failed\n");}
            printf("\nTotal malloc free tests Passed: %d/%d\n", passed_subtests, NUMMALTESTS);
            break;
        }
        case(2): {
            int num_of_tests;
            printf("How many Tests?");
            scanf("%d", &num_of_tests);
            speed_test(num_of_tests);
            break;
        }
        case(3): {
            error_test();         
        }

    }


    return EXIT_SUCCESS;
}
