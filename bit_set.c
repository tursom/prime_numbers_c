#pragma warning (disable:4819)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//#define _WIN32

#ifdef _WIN32
#include <Windows.h>
long getCurrentTime() {
    LARGE_INTEGER freq;
    QueryPerformanceCounter(&freq);
    return (long) freq.QuadPart / 10;
}
#else

#include <sys/time.h>

long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

#endif

/**
 * 判断字符串转换为无符号整型是否会溢出
 */
int willOverFlow(const char *numStr) {
    static char maxValueStr[64] = {'\0'};
    static const long negative1 = -1;
    static size_t lenOfMaxValue;
    if (*numStr == '-')return 1;
    if (!*maxValueStr) {
        sprintf(maxValueStr, "%lu", *(unsigned long *) &negative1);
        lenOfMaxValue = strlen(maxValueStr);
    }
    size_t numLen = strlen(numStr);
    return lenOfMaxValue < numLen ? 1 :
           lenOfMaxValue > numLen ? 0 :
           strcmp(maxValueStr, numStr) < 0;
}

size_t neededSize(unsigned long maxNumber) {
    return ((((maxNumber - 1) >> 7) + 1) & 0x0fffffffffffffff) << 3; // NOLINT(hicpp-signed-bitwise)
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

void getPrimeNumbers(unsigned long maxNumber, unsigned char *buffer) {
    size_t needSize = neededSize(maxNumber);
    unsigned char arr[] = {219, 182, 109};
    unsigned char initIndexArr[] = {1, 2, 0};
    unsigned char initIndex = 0;
    unsigned char *endP = buffer + needSize;

    // 初始化缓冲区
    for (unsigned char *k = buffer + 1; k < endP; ++k) {
        *k = arr[initIndex];
        initIndex = initIndexArr[initIndex];
    }
    *buffer = 0x6e;

    // 进行计算
    unsigned int sqrtMaxNumber = (unsigned int) sqrt(maxNumber) >> 1;
    maxNumber >>= 1;
    for (size_t i = 2; i <= sqrtMaxNumber; i++) {
        if (buffer[i >> 3] & 1 << (i & 7)) {
            size_t doubleI = i + i + 1;
            for (size_t j = (doubleI * doubleI) >> 1; j < maxNumber; j += doubleI) {
                buffer[j >> 3] &= ~(1 << (j & 7));
            }
        }
    }

    // 去除多余结果
    needSize <<= 3;
    for (int l = maxNumber; l < needSize; l++) {
        buffer[l >> 3] &= ~(1 << (l & 7));
    }
}

char ByteCode(const unsigned char n) {
    static const char table[256] = {
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
    };
    return table[n];
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

void printByteMap(unsigned char n) {
    printf("%d %d %d %d %d %d %d %d",
           (n & 1) != 0, (n & 0x2) != 0, (n & 0x4) != 0, (n & 0x8) != 0,
           (n & 0x10) != 0, (n & 0x20) != 0, (n & 0x40) != 0, (n & 0x80) != 0);
}

#pragma clang diagnostic pop

unsigned int getPrimeCount(const unsigned char *buff, size_t buffSize) {
    unsigned int primeCount = 1;
    for (int i = 0; i < buffSize; ++i) {
        primeCount += ByteCode(buff[i]);
    }
    return primeCount;
}

typedef char bool;
bool false = 0;
bool true = 1;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

bool isPrime(unsigned int num, const unsigned char *bitMap) {
    if (num < 2) {
        return false;
    } else if (num == 2) {
        return true;
    } else if (!(num & 1)) {
        return false; // 偶数
    } else {
        return bitMap[num >> 4] & (1 << ((num >> 1) & 7));
    }
}

#pragma clang diagnostic pop

/**
 * 程序入口函数
 */
int main(int argc, char *argv[]) {
    //必须给定最大值
    if (argc <= 1) {
        return 1;
    }

    //最大值
    unsigned long maxNum = 0;
    if (argc > 1) {
        //验证maxNum是否小于2，小于2的话计算没有意义
        //这里不能用maxNum验证，有负数的问题
        if (atol(argv[1]) < 2) { // NOLINT(cert-err34-c)
            return 2;
        }

        //判断最大值是否溢出
        if (willOverFlow(argv[1])) {
            //如果溢出
            //输出错误提示
            printf("too large maximum number");
            //程序退出
            //返回1表示最大值溢出
            return 1;
        }

        //如果都没有问题
        maxNum = (unsigned long) atol(argv[1]); // NOLINT(cert-err34-c)
    }

    int output = 1;
    if (argc > 2) {
        output = atoi(argv[2]); // NOLINT(cert-err34-c)
    }

    //分配内存，获得缓冲区;
    unsigned char *buff = malloc(neededSize(maxNum));

    //进行计算
    long t1 = getCurrentTime();
    getPrimeNumbers(maxNum, buff);
    long t2 = getCurrentTime();
    //输出
    if (output) {
        printf(maxNum < 100001 ? "2 " : "2\n");
        for (int i = 3; i <= maxNum; i += 2) {
//			if (buff[i >> 4] & (1 << ((i >> 1) & 7))) { // NOLINT(hicpp-signed-bitwise)
            if (isPrime(i, buff)) {
                printf(maxNum < 100001 ? "%d " : "%d\n", i);
            }
        }
    }
    printf("\nmax number: %li, find %u prime numbers\nusing time: %li s %li ms %li us\n",
           maxNum, getPrimeCount(buff, neededSize(maxNum)),
           (t2 - t1) / 1000 / 1000, (t2 - t1) / 1000 % 1000, (t2 - t1) % 1000);
    free(buff);
    return 0;
}
