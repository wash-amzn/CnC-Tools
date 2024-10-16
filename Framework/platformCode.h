#ifndef PLATFORMCODE_H
#define PLATFORMCODE_H
/*
 * Program Name: CnC Common Headers
 * File Name: platformCode.h
 * Date Created: January 21, 2024
 * Date Updated: January 29, 2024
 * Version: 0.1
 * Purpose: This file contains all functions that interact with platform-specific functionality
 */
#define _GNU_SOURCE // This is required for `pthread_setaffinity_np`
#include <pthread.h>
#ifdef __MINGW32__
#include <windows.h>
int getThreadCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
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
#elif __unix__
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
