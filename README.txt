Project: My Little Malloc (CS214)

Partners:
- Emily Cao (netID: ec1042)
- Ivan Zheng (netID: iz60)

Design notes:
- Our header: 
    - our header is an unsigned long long of an 8 byte bit string 
    - the allocation status is encoded in the first bit (1=allocated, 0=not)
    - the size in bytes is encoded in the last 4 bytes
- How our errors are formatted:
    - malloc reports an error when it occurs and returns NULL
    - free reports an error when it occurs and simply returns
- How we handle when there's only enough space for metadata:
    - we decided to simply give all the leftover space at the end of the memory to the data before it
    - this is because we are allowed to allocate more than requested, and it made the most logical sense
- Special Cases:
    - we chose not to allow for there to be headers pointing to no space as we believe it's unnecessary

Tests, Testing Plans, Testing Strategy:
- Our performance tests:
    - overall, we have seven performance tests, 3 of them being the three required from the write-up
    - past the three required, labeled as Task 1, Task 2, and Task 3, respectively, are four additional tests
    - standard_malloc_free() --> returns an int representing the status of being correct (1=correct, 0=incorrect)
        - This was the first test we wrote, intending to test out the basic functionality of malloc() and free()
        - the test allocates a big piece of data spanning memory, frees it, then allocates it again
    - malloc_max_number() --> returns an int representing total errors detected
        - Malloc an entire array of chars and count to make sure all the spaces have been used
        - Give each one a unique number and check that each one matches.
    - my_coalesce_test() --> returns an int representing the status of being correct (1=correct, 0=incorrect)
        - a basic coalescing test that malloc() 3 consecutive blocks of memory, frees them,
        then allocated data the size of the entire memory to make sure there no memory was lost.
    - free_coalesce_adj_test() --> returns an int representing the status of being correct (1=correct, 0=incorrect)
        - a more advanced coalescing test that fills up the memory with chars, then frees up every other char
        - it then goes back and frees up the other half
        - allows us to make sure coalesce is working properly in free() by successfully allocating a block of
        data spanning the memory
- Our Speed Tests:
	- We have 2 functions created for speed testing. (**NOTE: The Speed Tests allow for any number of tests to be run, whether that be 50 or more, part of the interface... See writeup below)
	- The first function is the default speed testing function > speed_test_default(int number_of_times)
		- designed to run a malloc_max_number() test and then a special free_max_number() test. This speeds of the tests will get averaged out over the x number of runs, and will print out the fastest run, the slowest run, and the average run for both malloc and free across 258 objects (max items that fit into our memory).
	- The second function > speed_test(int (*f)(), int number_of_times, int test_no)
		- Designed to take in any arbitrary test as a function pointer and run it the number of times specified. Prints out all the relevant information just like speed_test_default().
- Error Tests:
	- We have a few error tests to make sure the errors are printed out correctly for the 5 cases.
	- Case 1: Malloc Too Much Memory
	- Case 2: Malloc More than available memory
	- Case 3: Free Address of non-allocated object.
	- Case 4: Free address in heap but not of object.
	- Case 5: Free address outside heap.

How to Compile and Run our Test Cases:
- our makefile is included for your convenience
- compile and run our memgrind.c file by typing: "make memgrind"
    - clear the file space up with "make clean"
- We have makefile instructions for memtest included if necessary, and can be called using "make memtest"
- you will see a testing interface appear in the terminal
    - type in option 1 to run all of our performance tests
    - type in option 2 to get the speed data for all of our performance tests
    - type in option 3 to run all of our error tests


