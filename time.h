//
// Created by tursom on 2021/3/24.
//

#ifndef PRIME_NUMBERS_C_TIME_H
#define PRIME_NUMBERS_C_TIME_H

#ifdef _WIN32
#include <Windows.h>
long getCurrentTime() {
    LARGE_INTEGER freq;
    QueryPerformanceCounter(&freq);
    return (long) freq.QuadPart / 10;
}
#else

#include <sys/time.h>


typedef char bool;
bool false = 0;
bool true = 1;

long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

#endif

#endif //PRIME_NUMBERS_C_TIME_H
