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
#include <pthread.h>
#ifdef __MINGW32__
#include <windows.h>
int getThreadCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

int setAffinity(pthread_t thread)
{
    pthread_gethandle(thread);
}
#elif __unix__
#include <unistd.h>
int getThreadCount()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int setAffinity(pthread_t thread)
{

}
#endif


#endif // PLATFORMCODE_H
