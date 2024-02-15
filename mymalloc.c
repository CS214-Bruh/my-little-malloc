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
// For reading the first bit, returns 0 or 1 for allocated or not
#define readFirstBit(x) ((metadata) x << ((sizeof(unsigned long long)*8)-1))


// Metadata creation function
static metadata create_metadata(size_t byte_size, bool allocation) {
	// Create a 8 byte bit string with a 0 or 1 at the most significant bit for whether or not it is allocated.
	metadata x = (metadata) allocation << ((sizeof(unsigned long long)*8)-1);
	// Add on the byte size (in bytes) of the memory following to this 8 byte string.
	x += byte_size;
	return x;
}

// Memory initialization function for if the memory is not initialized.
static void init_memory(metadata* heapstart) {
	if(DEBUG) printf("Uninitialized, initializing...\n");
	metadata init_metadata = create_metadata(sizeof(double) * MEMLENGTH - sizeof(metadata), false);
	*heapstart = init_metadata;
	if(DEBUG) printf("%llu\n", *heapstart);
}

static unsigned int read_block_size(metadata *x) {
	int * ptr = (int *) x;
	return *(ptr+1);
}

void *mymalloc(size_t size, char *file, int line){
	if (DEBUG) printf("DEBUG MODE ON.\n");

	// Create a metadata-type pointer that will read the metadata at where the memory starts.
	metadata *heapstart = (metadata *) memory;
	
	// Checks the initialization of the memory.
	if((heapstart[0]|0x0000000000000000) == 0x0000000000000000) init_memory(heapstart);
	
	// While loop will go through the memory and look for a free (unallocated) block in order to write to.
	int memory_block = 0;
	// Can be less than MEMLENGTH because metadata is a 8-byte object, which will keep in alignment with the array.
	while(memory_block < MEMLENGTH) {
		if(readFirstBit(heapstart[memory_block]) == 1) {
			// Create 
			int* mem_block_counter = (int *) (memory + memory_block);
			int memory_block_size = 0;
			memory_block_size += *(mem_block_counter + 1);
			if(DEBUG) printf("Memory Block Size: %d\n", read_block_size(&heapstart[memory_block]));
			// Divide memory block into the 8-byte alignments and add 2. (1 for the metadata, and another to get to the beginning of the next)
			memory_block += (memory_block_size / 8) + 2;
			if(DEBUG) printf("%d\n", memory_block);
		} else {
			unsigned int mem_block_size_b = read_block_size(&heapstart[memory_block]);
			if(mem_block_size_b >= size) {
				// Create the replacement metadata
				metadata replacement = create_metadata(size, true);
				heapstart[memory_block] = replacement;
				// Make sure to move the pointer to where the end of the allocated block is and then make the new metadata for the unallocated remaining.
				// Save where the free block starts so we can return it.
				metadata* start_of_block_ret = &heapstart[memory_block];
				// This moves the memory block to the position to put the new free block.
				memory_block += (size / 8) +2;
				if(memory_block < MEMLENGTH) {
					// Creates the free metadata and puts it in the array.
					heapstart[memory_block] = create_metadata(mem_block_size_b - size, false);
				}
				return (void *) start_of_block_ret;
				}
			// If the memory block is not big enough, skip it and go to the next one.
			memory_block += (mem_block_size_b/8) +2;
			}
		}
	return (void *) heapstart;
}

void myfree(void *ptr, char *file, int line);

