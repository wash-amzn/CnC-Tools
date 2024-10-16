#ifndef PLATFORMCODE_H
#define PLATFORMCODE_H
/*
 * Program Name: CnC Common Headers
 * File Name: platformCode.h
 * Date Created: January 21, 2024
 * Date Updated: October 16, 2024
 * Version: 0.2
 * Purpose: This file contains all functions that interact with platform-specific functionality
 */

#ifdef __MINGW32__
// MINGW (at least on debian) expects _aligned_malloc, while gcc and clang expect aligned_alloc
#define aligned_alloc(sz, aln) _aligned_malloc(sz, aln)
#include <windows.h>
#include <pthread.h>
int getThreadCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

int setAffinity(pthread_t thread, int proc)
{
    HANDLE native_handle = pthread_gethandle(thread);
    // This isn't guaranteed to work on systems with more than 64 processors
    // To fix this we need to look into process groups
    DWORD mask = 1 << proc;

    return !SetProcessAffinityMask(
        native_handle,
        (DWORD_PTR)&mask
    );
}
#elif __unix__
#define _GNU_SOURCE // This is required for `CPU_ZERO`, and `CPU_SET`
#include <pthread.h>
#include <unistd.h>
int getThreadCount()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int setAffinity(pthread_t thread, int proc)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(proc, &cpuset);

    return pthread_setaffinity_np(
        thread,
        sizeof(cpu_set_t),
        &cpuset
    );
}
#endif


#endif // PLATFORMCODE_H
