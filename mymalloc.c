// mymalloc.c
// Imports for mymalloc
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

// Define booleans
// #define bool char
// #define FALSE 0
// #define TRUE 1
#include <stdbool.h>

#ifndef DEBUG
#define DEBUG 0
#endif

// Defining our memory array
# define MEMLENGTH 512
static double memory[MEMLENGTH];

// Defining the metadata
#define metadata unsigned long long
#define readFirstBit(x) ((metadata) x << ((sizeof(unsigned long long)*8)-1))


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
	if (DEBUG) printf("DEBUG MODE ON.\n");

	metadata *heapstart = (metadata *) memory;
	
	if((heapstart[0]|0x0000000000000000) == 0x0000000000000000) init_memory(heapstart);
	
	int memory_block = 0;
	while(memory_block < MEMLENGTH) {
		if(readFirstBit(heapstart[memory_block]) == 1) {
			int* mem_block_counter = (int *) (memory + memory_block);
			memory_block += *(mem_block_counter + 1);
			if(DEBUG) printf("%d\n", memory_block);
		} else {
			metadata replacement = create_metadata(size, true);
			heapstart[memory_block] = replacement;
			// Make sure to move the pointer to where the end of the allocated block is and then make the new metadata for the unallocated remaining.
			}
	}
	return (void *) heapstart;
}
	


void myfree(void *ptr, char *file, int line) {
	/*

	things to check for:
	- if free was called on a pointer not allocated my malloc
	- if free was called on an address not at the start of an address
	- if free was called on something already freed
	*/
	//case 1: check if the address is in the memory array

	//case 2: check if the address isn't a multiple of 8

	//case 3: check if the header says the space is already free

	//case 4: everything is passed
	readFirstBit(ptr) = 0;
	coalesce(ptr); //is this right

	
}

void coalesce(void *ptr) {
	//size of the current chunk
	int chunk_size = (metadata) x >> ((sizeof(unsigned long long)*8)-16);
	
	//flag to see if prev chunk was free
	int prev_status = 0;

	//traversing the memory
	bool memory_block = FALSE;
	while(memory_block < MEMLENGTH) {
		if(readFirstBit(heapstart[memory_block]) == TRUE) {
			//if the memory block is allocated, continue
	       		prev_status = 1;
				int* mem_block_counter = (int *) (memory + memory_block);
	       		memory_block += *(mem_block_counter + 1);
		} else {
			//found an empty space
			if (prev_status == FALSE) {
				int current_size = (metadata)  >> ((sizeof(unsigned long long)*8)-16);
				int* prev_header = (int *) (memory - memory_block);	//does memory block include the header?
				//make the current header into just a regular piece of memory
				//make the previous header point into a piece of memory that's the prev + the current size
			}
			//make a header for the end of it?
			metadata replacement = create_metadata(size, true);
			heapstart[memory_block] = replacement;
		 	// Make sure to move the pointer to where the end of the allocated block is and then make the new metadata for the unallocated remaining.
         }
       }
     }



