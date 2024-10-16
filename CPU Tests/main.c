/*
 * Program Name: CoreCoherencyTest
 * File Name: main.c
 * Date Created: January 21, 2024
 * Date Updated: February 5, 2024
 * Version: 0.1.1
 * Purpose: Test Core-to-Core Latency of Multi-Core CPU's using Coherency checks.
 */

#include "platformCode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "platformCode.h"
#include "timing.h"

void testFunc();
int iterations = 100000;//This probably needs adjusting

pthread_t testThreads[2];

int main()
{
    //Allocate 4096 bytes of storage with a 64byte alignment
    uint64_t *data = (uint64_t *) aligned_alloc(4096, 64);

    printf("Core Coherency Test\n");

    //Fetch and Display Thread Count.  If this fails end program execution
    int threadCount = getThreadCount();
    if(threadCount <= 0)
    {
        printf("Thread Count was not successfully acquired.  Test execution has been skipped.\n");
        return -1;
    }

    printf("CPU Thread Count: %d\n", threadCount);
    printf("Time to run %dns\n", time_execution(testFunc, 1));

    return 0;
}

//Dummy function to test timing
void testFunc()
{
    volatile long a = 2;
    volatile long b = 2;
    for(long int i = 0; i < 25000; i++)
    {
        for(int z = 0; i < 10; i++)
        {
            a = a + b;
        }
        b = a + b;
    }
}
