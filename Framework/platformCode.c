#include "platformCode.h"
/*
 * Program Name: CnC Common Headers
 * File Name: platformCode.c
 * Date Created: October 27, 2024
 * Date Updated: November 9, 2024
 * Version: 0.4.1
 * Purpose: This file contains all functions that interact with platform-specific functionality
 */

//SEE HEADER FILE FOR FUNCTION DOCUMENTATION

#ifdef __MINGW32__

int *AFFINITY = NULL;

int getThreadCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    if (AFFINITY == NULL) {
        int thread_count = sysinfo.dwNumberOfProcessors;
        AFFINITY = (int *) malloc(thread_count * sizeof(int));
        memset(AFFINITY, 0, thread_count * sizeof(int));
    }
    return sysinfo.dwNumberOfProcessors;
}

int getAffinity(pthread_t thread)
{
    if (AFFINITY == NULL) {
        int thread_count = getThreadCount();
        AFFINITY = (int *) malloc(thread_count * sizeof(int));
        memset(AFFINITY, 0, thread_count * sizeof(int));
        return 0;
    }
    return AFFINITY[thread];
}

int setAffinity(pthread_t thread, int proc)
{
    if (AFFINITY == NULL) {
        int thread_count = getThreadCount();
        AFFINITY = (int *) malloc(thread_count * sizeof(int));
        memset(AFFINITY, 0, thread_count * sizeof(int));
    }
    
    // This isn't guaranteed to work on systems with more than 64 processors
    // To fix this we need to look into process groups
    DWORD_PTR mask = 1 << proc;
    int status = SetProcessAffinityMask(pthread_gethandle(thread), mask);

    // Set affinity in our array
    if (status != 0) 
        AFFINITY[thread] = proc;
    
    return !status;
}
#elif __unix__

int getThreadCount()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int getAffinity(pthread_t thread, int proc)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(proc, &cpuset);

    return pthread_getaffinity_np(thread, sizeof(cpu_set_t),&cpuset);
}

int setAffinity(pthread_t thread, int proc)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(proc, &cpuset);

    return pthread_setaffinity_np(thread, sizeof(cpu_set_t),&cpuset);
}
#endif
