# My Little Malloc

Creation of a `mymalloc()` library for Rutgers University class: `01:198:214` - Systems Programming

Created by: **Emily Cao** (ec1042) and **Ivan Zheng** (iz60)

```mermaid
classDiagram
    class mymalloc {
        char[] things
        int we_need
        char to_populate
    }

	class myfree {
		char[] things
		int we_need
		char to_populate
	}
```

## Errors to Detect

1. Attempting to `malloc()` too much memory.
	- Return where the error occurred and how much they tried to allocate (_Attempt to allocate BYTE_SIZE from Line LINE_NUM_)
2. Attempt to `malloc()` more memory than is currently available.
	- Error occurs when allocating more memory that may not be available in a row.
3. Attempt to free an address outside the heap.
	- Return the memory address ex: trying to free a local variable.
4. Attempt to free an address of a non-allocated objected. (Already freed object)
5. Atempt to free an address in the heap but not of an object.
	- Ex: having an array in the heap but trying to free just the 2nd object in the heap.

---

## Important Variables

```C
#define MEMLENGTH 512
static double memory[MEMLENGTH];	// Usage of "static" makes the array private to this file.
// That is, it doesn't participate in linking
// We use "double" because it is 8-bytes and has 8-byte alignment.
```

## Steps

### Program Start
- Memory should be a single free block.
- When you get a call to `malloc()`, you should look through the memory and look for a portion of free blocks the object can fit into.
- As `malloc()` proceeds, we keep dividing the memory up into more and more blocks. (Don't forget the metadata)
- **Make sure you know where the memory begins and how big it is.**
- **The free block also requires metadata**
