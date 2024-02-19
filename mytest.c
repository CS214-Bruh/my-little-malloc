#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

#define NANOSECONDS 1000000000.0
#define NUMTESTS 50 // Indicates how many times we want to stress test our code, set as desired


// THE FOLLOWING SECTION IS ALL CODE FOR DEBUGGING PURPOSES
// ---------------------------------------------------------


// Uncomment following line for debugging purposes

// #define DEBUG 1

// Helper method for debugging TEST 3
#ifdef DEBUG
void printIndexAllocated(int i)
{
    printf("indexAllocated = %d\n", i);
};
#endif
// ---------------------------------------------------------


/// @brief Prints and returns average time taken over "size" iterations
/// @param arrTest Array of test runtimes
/// @param size Size of arrTest
/// @param stressTestNumber Test number to be printed 
/// @return Average of arrTest 

double printAvgTest(double *arrTest, int size, int stressTestNumber)
{
    double runningSum = 0;
    for (int i = 0; i < size; i++)
    {
        runningSum += arrTest[i];
    }
    double totalSum = runningSum / ((double)size);
    printf("AVG RUNTIME of TEST %d over %d iterations: %.10lf seconds\n", stressTestNumber, size, totalSum);
    return totalSum;
}

int main()
{
    srand(time(NULL));

    struct timespec startTime;
    struct timespec endTime;

    double testRuntimes[NUMTESTS];

    for (int testNo = 0; testNo < NUMTESTS; testNo++)
    {
        timespec_get(&startTime, TIME_UTC);
        for (int i = 0; i < 120; i++)
        {
            void *testptr = malloc(1);
            free(testptr);
        }
        timespec_get(&endTime, TIME_UTC);
        testRuntimes[testNo] = (endTime.tv_nsec - startTime.tv_nsec) / NANOSECONDS;
    }

    printAvgTest(testRuntimes, NUMTESTS, 1);

    // TEST TWO
    for (int testNo = 0; testNo < NUMTESTS; testNo++)
    {
        timespec_get(&startTime, TIME_UTC);
        void **ptrTestTwo = (void **)malloc((sizeof(void *) * 120));

        for (int j = 0; j < 100; j++)
        {
            for (int i = 0; i < 120; i++)
            {
                ptrTestTwo[i] = malloc(1);
            }
            for (int i = 0; i < 120; i++)
            {
                free(ptrTestTwo[i]);
            }
        }

        free(ptrTestTwo);
        timespec_get(&endTime, TIME_UTC);
        testRuntimes[testNo] = (endTime.tv_nsec - startTime.tv_nsec) / NANOSECONDS;
    }

    printAvgTest(testRuntimes, NUMTESTS, 2);

    // TEST THREE
    for (int testNo = 0; testNo < NUMTESTS; testNo++)
    {
        timespec_get(&startTime, TIME_UTC);

        int allocateCounter = 0;
        int indexAllocated = 0;
        void **ptrTestThree = (void **)malloc(sizeof(void *) * 120);

        while (allocateCounter < 120)
        {
            int r = rand();
            if (r % 2 == 0)
            {
                if (indexAllocated < 0)
                    indexAllocated = 0;
                ptrTestThree[indexAllocated] = malloc(1);
                allocateCounter++;
                indexAllocated++;
#ifdef DEBUG
                printIndexAllocated();
#endif
            }
            else
            {
                if (indexAllocated > 0)
                {
                    free(ptrTestThree[indexAllocated - 1]);
                    indexAllocated--;
#ifdef DEBUG
                    printIndexAllocated();
#endif
                }
#ifdef DEBUG
                else
                {
                    printf("Nothing in array\n");
                }
#endif
            }
        }

        for (int i = 0; i < indexAllocated; i++)
        {
            free(ptrTestThree[i]);
#ifdef DEBUG
            printf("SUCCESSFUL DEALLOCATION AT LOC %d\n", i);
#endif
        }

        free(ptrTestThree);
        timespec_get(&endTime, TIME_UTC);
        testRuntimes[testNo] = (endTime.tv_nsec - startTime.tv_nsec) / NANOSECONDS;
    }

    printAvgTest(testRuntimes,NUMTESTS,3); 

    // TEST 4
    // Description of Test 4: We will be testing the coalescing
    // abilities of our code, by allocating a contiguous array
    // of pointers, then free'ing only the odd-numbered pointers
    // leaving a fragmented array that will coalesce when we free
    // the even-numbered pointers
    for (int testNo = 0; testNo < NUMTESTS; testNo++)
    {
        timespec_get(&startTime, TIME_UTC);

        void **ptrTestFour = (void **)malloc(sizeof(void *) * 120);
        for (int i = 0; i < 120; i++)
        {
            ptrTestFour[i] = malloc(1);
        }
        for (int i = 0; i < 120; i += 2)
        {
            free(ptrTestFour[i]);
        }
        for (int i = 1; i < 120; i += 2)
        {
            free(ptrTestFour[i]);
        }
        free(ptrTestFour);
        timespec_get(&endTime, TIME_UTC);
        testRuntimes[testNo] = (endTime.tv_nsec - startTime.tv_nsec) / NANOSECONDS;
    }

    printAvgTest(testRuntimes, NUMTESTS, 4);

    // TEST 5: SEE README.TXT FOR FULL DESCRIPTION
    // We will create an array of pointers that will be free'd
    // from both ends, coalescing into larger and larger blocks at each
    // endpt, until both merge into one. Which one is faster? Let's find
    // out!

    printf("---\nSee source code for explanation of Test 5\n---\n"); 
    double testRuntimesLeft[NUMTESTS];
    double testRuntimesRight[NUMTESTS];
    for (int testNo = 0; testNo < NUMTESTS; testNo++)

    {
        struct timespec startTimeLeft, startTimeRight, endTimeLeft, endTimeRight;

        int sizeOfList = 120;

        timespec_get(&startTime, TIME_UTC);

        void **ptrTestFive = (void **)malloc(sizeof(void *) * 120);
        for (int i = 0; i < sizeOfList; i++)
        {
            ptrTestFive[i] = malloc(1);
        }
        timespec_get(&startTimeLeft, TIME_UTC);

        for (int i = 0; i < sizeOfList / 2; i++)
        {
            free(ptrTestFive[i]);
        }
        timespec_get(&endTimeLeft, TIME_UTC);
        timespec_get(&startTimeRight, TIME_UTC);

        for (int i = sizeOfList - 1; i > sizeOfList / 2 - 1; i--)
        {
            free(ptrTestFive[i]);
        }

        free(ptrTestFive);

        timespec_get(&endTimeRight, TIME_UTC);
        timespec_get(&endTime, TIME_UTC);
        testRuntimes[testNo] = (endTime.tv_nsec - startTime.tv_nsec) / NANOSECONDS;
        testRuntimesLeft[testNo] = (endTimeLeft.tv_nsec - startTimeLeft.tv_nsec) / NANOSECONDS;
        testRuntimesRight[testNo] = (endTimeRight.tv_nsec - startTimeRight.tv_nsec) / NANOSECONDS;
    }
    printf("LEFT    ");
    printAvgTest(testRuntimesLeft, NUMTESTS, 5);
    printf("RIGHT   ");
    printAvgTest(testRuntimesRight, NUMTESTS, 5);
    printf("OVERALL ");
    printAvgTest(testRuntimes, NUMTESTS, 5);

    // If we run this program, indeed, we'll find that the right
    // average runtime is an order of magnitude larger than the
    // left average runtime, since coalescing on the left direction
    // doesn't require traversing the entire list to coalesce,
    // whereas the right hand version does.

    memLeak();
    return 1;
}
