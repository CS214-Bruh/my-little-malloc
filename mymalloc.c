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

// Metadata creation function
static metadata create_metadata(size_t byte_size, bool allocation) {
	// Create a 8 byte bit string with a 0 or 1 at the most significant bit for whether or not it is allocated.
	metadata x = (metadata) allocation << ((sizeof(unsigned long long)*8)-1);
	// Add on the byte size (in bytes) of the memory following to this 8 byte string.
    int thing = byte_size;
    if(byte_size%8 != 0) {
        byte_size += (8-(byte_size%8));
    }
	x += byte_size;
//    if(DEBUG) {
//        printf("Allocation: %b, Metadata val: %llu, Byte Size: %zu\n", allocation, x, byte_size);
//    }
	return x;
}

// Memory initialization function for if the memory is not initialized.
static void init_memory(metadata** heapstart) {
	if(DEBUG) printf("Uninitialized, initializing...\n");
	metadata init_metadata = create_metadata(sizeof(double) * MEMLENGTH - sizeof(metadata), false);
	*heapstart[0] = init_metadata;
	// if(DEBUG) printf("%llu\n", *heapstart[0]);
}

// For reading the first bit, returns 0 or 1 for allocated or not
static bool read_first_bit(metadata x) {
    return (x >> ((sizeof(unsigned long long)*8)-1));
}

// Broken
// TODO: Fix it
static metadata read_block_size(metadata x) {
    unsigned long long mask = 0x8000000000000000;
    x ^= mask;
//    if(DEBUG) printf("Size (Bytes): %llu\n", x);

//    unsigned int memory_block_size = (mem_block_counter[1]);
//    printf("Yes2: %d\n",memory_block_size);
    return x;
}

void *mymalloc(size_t size, char *file, int line){
	// if (DEBUG) printf("DEBUG MODE ON.\n");

	// Create a metadata-type pointer that will read the metadata at where the memory starts.
	metadata *heapstart = (metadata *) memory;

	// Checks the initialization of the memory.
	if((heapstart[0]|0x0000000000000000) == 0x0000000000000000) init_memory(&heapstart);

    // While loop will go through the memory and look for a free (unallocated) block in order to write to.
	unsigned int memory_block = 0;
	// Can be less than MEMLENGTH because metadata is a 8-byte object, which will keep in alignment with the array.
	while(memory_block < MEMLENGTH) {
        // If the first bit is already allocated, move to the start of the next object
//        if(DEBUG) {
//            printf("Metadata value: %llu, Memory Block Number: %d, Bit allocated: %b\n", heapstart[memory_block], memory_block,
//                   read_first_bit(heapstart[memory_block]));
//        }

		if(read_first_bit(heapstart[memory_block]) == 1) {
			// if(DEBUG) printf("Memory Block Size: %d\n", read_block_size(&heapstart[memory_block]));
			// Divide memory block into the 8-byte alignments and add 1.
			memory_block += (read_block_size(heapstart[memory_block]) / 8)+1;
			if(DEBUG) printf("Block Not Empty, Moving to block: %d\n", memory_block);
		} else {
			unsigned int mem_block_size_b = heapstart[memory_block];
            if(DEBUG) {
                printf("Found free block of size: %d, (Allocated: %i)\n", mem_block_size_b, read_first_bit(
                        read_first_bit(heapstart[memory_block])));
            }
			if(mem_block_size_b >= size) {
				// Create the replacement metadata
				metadata replacement = create_metadata(size, true);
                if(DEBUG) printf("Creating new block..\nNew Block Size: %llu, New Metadata (%llu) inserted at Block: %d\n", read_block_size(replacement), replacement, memory_block);
                heapstart[memory_block] = replacement;
				// Make sure to move the pointer to where the end of the allocated block is and then make the new metadata for the unallocated remaining.
				// Save where the free block starts, so we can return it.
				metadata* start_of_block_ret = &(heapstart[memory_block+1]);

                if(DEBUG) printf("Replaced free block metadata with new metadata of block size: %llu\n", (read_block_size(heapstart[memory_block]) / 8));
				// This moves the memory block to the position to put the new free block.
				memory_block += (read_block_size(heapstart[memory_block]) / 8) + 1;
                if(DEBUG) printf("Memory Block is on Number: %i\n", memory_block);

                if(memory_block < MEMLENGTH) {
					// Creates the free metadata and puts it in the array.
					heapstart[memory_block] = create_metadata(mem_block_size_b - size - sizeof(metadata), false);
                    if(DEBUG) printf("Creating new FREE block..\nNew Block Size: %llu, New Metadata (%llu) inserted at Block: %d\n", heapstart[memory_block], heapstart[memory_block], memory_block);
				}
				return (void *) start_of_block_ret;
				} else {
                // If the memory block is not big enough, skip it and go to the next one.
                memory_block += (read_block_size(heapstart[memory_block]) / 8 )+1;
            }
        }
        if(DEBUG) printf("Current memory block is: %d\n", memory_block);
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



