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

// Defining the metadata and max int size
#define metadata unsigned long long
#define MAX_INT 2147483647

// Metadata creation function
static metadata create_metadata(size_t byte_size, bool allocation) {
	// Create a 8 byte bit string with a 0 or 1 at the most significant bit for whether or not it is allocated.
	metadata x = (metadata) allocation << ((sizeof(unsigned long long)*8)-1);
	// Add on the byte size (in bytes) of the memory following to this 8 byte string.
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
    if(x > MAX_INT) {
        unsigned long long mask = 0x8000000000000000;
        x ^= mask;
    }
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

    // Error Check 1: Malloc too much memory
    if(size > read_block_size(heapstart[0])) {
        fprintf(stderr, "Attempt to allocate %zu bytes from %s : Line %d. \nNo memory allocated.\n", size, file, line);
        if(DEBUG) printf("Actual memory size: %llu\n", read_block_size(heapstart[0]));
        return NULL;
    }

	// Can be less than MEMLENGTH because metadata is a 8-byte object, which will keep in alignment with the array.
	while(memory_block < MEMLENGTH) {
        // If the first bit is already allocated, move to the start of the next object
		if(read_first_bit(heapstart[memory_block]) == 1) {
			// if(DEBUG) printf("Memory Block Size: %d\n", read_block_size(&heapstart[memory_block]));
			// Divide memory block into the 8-byte alignments and add 1.
			memory_block += (read_block_size(heapstart[memory_block]) / 8)+1;
			// if(DEBUG) printf("Block Not Empty, Moving to block: %d\n", memory_block);
		} else {
			unsigned int mem_block_size_b = heapstart[memory_block];
            if(DEBUG) {printf("Found free block of size: %d, (Allocated: %i)\n", mem_block_size_b,
                        read_first_bit(heapstart[memory_block]));}
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
                if(DEBUG) printf("Memory Block is on Number: %i\n\n", memory_block);

                // Once the new allocated metadata is made, we need to create a new free metadata.
                // TODO: Discuss how to handle cases where there is only 1 byte left. (Enough for header and nothing else)
                if(memory_block < MEMLENGTH) {
					// Creates the free metadata and puts it in the array.
					heapstart[memory_block] = create_metadata(mem_block_size_b - size - sizeof(metadata), false);
                    if(DEBUG) printf("Creating new FREE block..\nNew Block Size: %llu, New Metadata (%llu) inserted at Block: %d\n",
                                     read_block_size(heapstart[memory_block]), heapstart[memory_block], memory_block);
				} else {if(DEBUG) printf("No free blocks left. %d is >= %d\n", memory_block, MEMLENGTH);}
				return (void *) start_of_block_ret;
				} else {
                // If the memory block is not big enough, skip it and go to the next one.
                memory_block += (read_block_size(heapstart[memory_block]) / 8 )+1;
            }
        }
    }

    // Error 2: Attempted to malloc more memory than currently available.
    fprintf(stderr, "Not enough memory available from call to malloc from %s: Line %d.\n", file, line);
    return NULL;
}

void coalesce(metadata *ptr) {
    //flag to see if prev chunk was free, true if free
    int prev_status = false;

    // Create a metadata-type pointer that will read the metadata at where the memory starts.
	metadata *heapstart = (metadata *) memory;

    //block we're currently in
    unsigned int current_block = 0;

    //prev block info
    int prev_size = 0; 

    //header of previous free block
    unsigned int prev_block = 0;


    //traversing the memory
    while(current_block < MEMLENGTH) {
       
        if(read_first_bit(heapstart[current_block]) == true) {
            //move to next block if not free
            current_block += (read_block_size(heapstart[current_block]) / 8)+1;

            //set status
            prev_status = false;
        } else { 
            //free block
            if (prev_status == true) {
                //prev was free too
                int new_size = read_block_size(heapstart[current_block]) + prev_size;

                //current block size
                int current_size = read_block_size(heapstart[current_block]);

                //change size on prev block header
                metadata new_header = create_metadata(new_size, 0);
                heapstart[prev_block] = new_header;

                //set the current header to all 0's
                heapstart[current_block] = 0x0000000000000000;
                
                //move to the next block
                current_block += (current_size / 8)+1;
                

            } else {
                //prev wasn't free
                //set status
                prev_status = true;

                //set prev size
                prev_size = read_block_size(heapstart[current_block]); 

                //set prev header
                prev_block = current_block;

                //move to next block
                current_block += (read_block_size(heapstart[current_block]) / 8)+1;
            }
            
        }
    }
}

void myfree(void *ptr, char *file, int line) {
	/*

	things to check for:
	- if free was called on a pointer not allocated my malloc
	- if free was called on an address not at the start of an address
	- if free was called on something already freed
	*/
    //cast ptr
    metadata new_ptr = (metadata) ptr;

	//case 1: check if the address is in the memory array
    //case 2: check if the address isn't at start

    // Create a metadata-type pointer that will read the metadata at where the memory starts.
    bool in_array = false;
	metadata *heapstart = (metadata *) memory;

    //block we are in
    unsigned int current_block = 0;

    //traversing memory to find address
    while(current_block < MEMLENGTH) {
       
        if(heapstart[current_block] + 1 == new_ptr) {
            //move to next block if not free
            in_array = true;
            break;
        } 
        current_block += (read_block_size(heapstart[current_block]) / 8)+1;
    }

    if (!in_array) {
        fprintf(stderr, "Free called on pointer not allocated by malloc on %s: Line %d.\n", file, line);
        return;
    }

	//case 3: check if the header says the space is already free
    if (read_first_bit(*(metadata *)ptr) == 0) {
        fprintf(stderr, "Free called on pointer already freed on %s: Line %d.\n", file, line);
        return;
    }

	//case 4: everything is passed (read_first_bit returns a 1 or 0.)
	//read_first_bit(*(metadata *)ptr);

    metadata new_header = create_metadata(read_block_size(new_ptr), false);
    new_ptr = new_header; //free up the space
	coalesce(heapstart);

}





