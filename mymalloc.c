// mymalloc.c
// Imports for mymalloc
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

// Define booleans
// #define BOOL char
// #define FALSE 0
// #define TRUE 1
#include <stdbool.h>

#ifndef DEBUG
#define DEBUG 0
#else
printf("DEBUG MODE ON.")
#endif

// Defining our memory array
# define MEMLENGTH 512
static double memory[MEMLENGTH];

// Defining the metadata
#define metadata unsigned long long

// Metadata creation function
metadata create_metadata(size_t byte_size, bool allocation) {
	metadata x = (metadata) allocation << ((sizeof(unsigned long long)*8)-1);
	x += byte_size;
	return x;
}

void init_memory(metadata* heapstart) {
	if(DEBUG) printf("Uninitialized, initializing...\n");
	metadata init_metadata = create_metadata(sizeof(double) * MEMLENGTH - sizeof(metadata), false);
	*heapstart = init_metadata;
	if(DEBUG) printf("%llu\n", *heapstart);
}

void *mymalloc(size_t size, char *file, int line){
	metadata *heapstart = (metadata *) memory;
	
	if((heapstart[0]|0x0000000000000000) == 0x0000000000000000) init_memory(heapstart);
	return (void *) heapstart;
}

void myfree(void *ptr, char *file, int line);

