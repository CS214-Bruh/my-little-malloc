Project: My Little Malloc (CS214)

Partners:
- Emily Cao (netID: ec1042)
- Ivan Zheng (netID: iz60)

Design notes:
- Our header: 
    - our header is an unsigned long long of an 8 byte bit string 
    - the allocation status is encoded in the first bit (1=allocated, 0=not)
    - the size in bytes is encoded in the last 2 bits
- How our errors are formatted:
    - malloc reports an error when it occurs and returns NULL
    - free reports an error when it occurs and simply returns
- How we handle when there's only enough space for metadata:
    - we decided to simply all the leftover space at the end of the memory towards the data before it
    - this is because we are allowed to allocate more than requested and it made the most logical sense
- Special Cases:
    - we chose not to allow for there to be headers pointing to no space as we believe it's unnecessary

Tests, Testing Plans, Testing Strategy:
- Our performance tests:
    - overall we have 7 performance tests, 3 of them being the 3 required from the write up
    - past the 3 required, labeled as Task 1, Task 2, and Task 3, respectively, are 4 additional tests
    - standard_malloc_free() --> returns an int representing the status of being correct (1=correct, 0=incorrect)
        - This was the first test we wrote, intending to test out the basic functionality of malloc() and free()
        - the test allocates a big piece of data spanning memory, frees it, then allocates it again
    - malloc_max_number() --> returns an int representing total errors detected
        - Malloc an entire array of chars and count to make sure the all the spaces have been used
        - All the bytes for headers and payload should add up to the MEMSIZE
    - my_coalesce_test() --> returns an int representing the status of being correct (1=correct, 0=incorrect)
        - a basic coalescing test that malloc() 3 consecutive blocks of memory, frees them,
        then allocated data the size of the entire memory
    - free_coalesce_adj_test() --> returns an int representing the status of being correct (1=correct, 0=incorrect)
        - a more advanced coalescing test that fills up the memory with chars, then frees up every other char
        - it then goes back and frees up the other half
        - allows us to make sure coalesce is working properly in free() by successfully allocating a block of
        data spanning the memory

How to Compile and Run our Test Cases:
- our makefile is included for your convenience
- compile and run our memgrind.c file by typing: "make memgrind"
    - clear the file space up with "make clean"
- you will see a testing interface appear in the terminal
    - type in option 1 to run all of our performance tests
    - type in option 2 to get the speed data for all of our performance tests
    - type in option 3 to run all of our error tests




 
- any additional info needed to compile and run our test cases
	- like how to use them, what info they take, etc.
- design notes
	- for situation where we just have enough space for a metadata: stretches the previous one up to the end
- our test plan/testing strategy
- description of our performance tests and why we chose them
- we don't have to submit memtest
- like what fields we have in our header & why
- what we do during errors (like if we just return or whatever)
	- malloc must return null and continue on
	- free can do anything (message and return or whatever)