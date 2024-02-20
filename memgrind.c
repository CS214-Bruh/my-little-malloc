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

#define NUMMALTESTS 7
#define MAX_INT 2147483647

unsigned char *arr[BLOCKSIZE/2];

//Task 1: malloc() & immediately free() a 1-byte object, 120 times
int malloc_free_120() {
   for (int i = 0; i < 120; i++) {
        char* ptr = malloc(sizeof(char));
        free(ptr);
    }
   int correct =1;
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

//Task 2: array of 120 pointers to 1-byte objects through malloc, then freed
int malloc120_free() {
    //filling up the array
    char *arr_ptrs[120];
    for(int i = 0; i < 120; i++) {
        arr_ptrs[i] = malloc(sizeof(char));
    }
    
    //freeing the array up
    for(int i = 0; i < 120; i++) {
        free(arr_ptrs[i]);
    }

    int correct =1;
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

//Task 3: array of 120 pointers, randomly choose to allocate or deallocate until 
// 120 total allocations have been done, deallocate any remaining objects
int random_malloc_free() {
    int total_allocations = 0;
   // Array of ints indicating 0 if not allocated, 1 if allocated.
    int allocated[120];
   for(int i = 0; i < 120; i++) {
      allocated[i] = 0;
   }
    int location = 0;
    char *arr_ptrs[120];

    while (total_allocations < 120) {
        int rand_val = rand() % 2; //generate a random val b/w 0 and 1, 0 indicates deallocate, 1 indicates allocate
        if (rand_val == 1) {
            //printf("the allocated: %d\n", location);
            arr_ptrs[location] = malloc(sizeof(char));
            //set to arbitrary value as allocated array initialized with indeterminate values
            allocated[location] = 1;
            total_allocations++;
            location++;
        } else if (rand_val == 0 && location != 0) {
           free(arr_ptrs[location]);
            location--;
            total_allocations--;
            allocated[location] = 0;
            //printf("freed: %d\n", location);
        }
    }

    //clean up any remaining objects
    for(int i = 0; i < total_allocations; i++) {
        if (allocated[i] == 1) {
            //printf("the still allocated: %d\n", i);
            free(arr_ptrs[i]);
        }
    }
    return 1;
}


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
struct timeval malloc_start, malloc_stop, free_start, free_stop, other_start, other_stop;

void speed_test_default(int number_of_times) {
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

void speed_test(int (*f)(), int number_of_times, int test_no){
    double other_avg = 0;
    double other_top = 0;
    double other_low = MAX_INT;

    int i = 0;
    while(i < number_of_times) {
        gettimeofday(&other_start, NULL);
        f();
        gettimeofday(&other_stop, NULL);
        double other_time =
                ((other_stop.tv_sec - other_start.tv_sec) * 1000000 + other_stop.tv_usec - other_start.tv_usec) *
                0.001;
        if (other_time > other_top) other_top = other_time;
        if (other_time < other_low) other_low = other_time;
        other_avg += other_time;
        i++;
    }

    printf("Test %d Statistics:\n", test_no);
    printf("Total Items Malloc-ed: %d\n", (BLOCKSIZE/2)*number_of_times);
    printf("Average Malloc-ing Time per Run of %d objects: %lf\n", BLOCKSIZE/2, other_avg/number_of_times);
    printf("Slowest Run of %d objects: %lf\n", BLOCKSIZE/2, other_top);
    printf("Fastest Run of %d objects: %lf\n", BLOCKSIZE/2, other_low);
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
    printf("\n");

    printf("Malloc Error Test 2: Malloc More Than Available Memory\n");
    char* ptr = malloc(sizeof(char) * MEMSIZE-HEADERSIZE-1);
    char* ptr2 = malloc(sizeof(char));
    printf("\n");

    printf("Free Error Test 1: Free called on pointer not allocated by malloc \n");
    int value = 5;
    int* value_ptr = &value;
    free(value_ptr);
    printf("\n");

    printf("Free Error Test 2: Free called on pointer already freed \n");
    free(ptr);
    char* ptr3 = malloc(sizeof(char) * MEMSIZE-HEADERSIZE-1);
    free(ptr3);
    free(ptr3);
    printf("\n");

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
            printf("Running Task 1: malloc() & immediately free() a 1-byte object, 120 times...");
            if(malloc_free_120()){printf("Passed\n");passed_subtests++;}else{ printf("Failed\n");}
            printf("Running Task 2: malloc() 120 1-byte objects into an array of pointers, then freeing...");
            if(malloc120_free()){printf("Passed\n");passed_subtests++;}else{ printf("Failed\n");}
            printf("Running Task 3: randomly free and allocate from an array of 120 pointer to 1-byte objects...");
            if(random_malloc_free()){printf("Passed\n");passed_subtests++;}else{ printf("Failed\n");}
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
            int numb_of_tests;
            printf("How many Tests?");
            scanf("%d", &numb_of_tests);
            speed_test_default(numb_of_tests);
            speed_test(&malloc_free_120, numb_of_tests, 1);
            speed_test(&malloc120_free, numb_of_tests, 2);
            speed_test(&random_malloc_free, numb_of_tests, 3);
            break;
        }
        case(3): {
            error_test();         
        }

    }


    return EXIT_SUCCESS;
}
