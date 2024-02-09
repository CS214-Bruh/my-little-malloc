// mymalloc.c
// Imports for mymalloc
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

// Define booleans
#define BOOL char
#define FALSE 0
#define TRUE 1
// #include <stdbool.h>

// Define things
typedef struct {
	BOOL allocated;
	unsigned short size;
}

void *mymalloc(size_t size, char *file, int line);

void myfree(void *ptr, char *file, int line);
