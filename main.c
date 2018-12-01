#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <event.h>

/**
 * 判断字符串转换为无符号整型是否会溢出
 */
int willOverFlow(char *numStr) {
	static char maxValueStr[32] = {'\0'};
	static const int negative1 = -1;
	static size_t lenOfMaxValue;
	if (*numStr == '-')return 1;
	if (!*maxValueStr) {
		sprintf(maxValueStr, "%u", *(unsigned int *) &negative1);
		lenOfMaxValue = strlen(maxValueStr);
	}
	size_t numLen = strlen(numStr);
	return lenOfMaxValue < numLen ? 1 :
	       lenOfMaxValue > numLen ? 0 :
	       strcmp(maxValueStr, numStr) < 0;
}

size_t neededSize(unsigned long maxNumber) {
	return ((maxNumber - 1) >> 4) + 1;
}

void goGetPrimeNumbers(unsigned int maxNumber, unsigned char *buffer) {
	for (int k = 1; k < neededSize(maxNumber); ++k) {
		buffer[k] = 0xff;
	}
	*buffer = 0xfe;
	unsigned int sqrtMaxNumber = (unsigned int) sqrt(maxNumber);
	for (size_t i = 3; i <= sqrtMaxNumber; i += 2) {
		if (buffer[i >> 4] & 1 << ((i >> 1) & 7)) {
			for (size_t j = i + i + i; j <= maxNumber; j += i + i) {
				buffer[j >> 4] &= ~(1 << ((j >> 1) & 7));
			}
		}
	}
	for (int l = maxNumber; l < neededSize(maxNumber) << 4; l++) {
		buffer[l >> 4] &= ~(1 << ((l >> 1) & 7));
	}
}

long getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

char ByteCode(unsigned char n) {
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

void printByteMap(unsigned char n) {
	printf("%d %d %d %d %d %d %d %d",
	       (n & 1) != 0, (n & 0x2) != 0, (n & 0x4) != 0, (n & 0x8) != 0,
	       (n & 0x10) != 0, (n & 0x20) != 0, (n & 0x40) != 0, (n & 0x80) != 0);
}

unsigned int getPrimeCount(unsigned char *buff, size_t buffSize) {
	unsigned int primeCount = 1;
	for (int i = 0; i < buffSize; ++i) {
		primeCount += ByteCode(buff[i]);
	}
	return primeCount;
}

/**
 * 程序入口函数
 */
int main(int argc, char *argv[]) {
	//必须给定最大值
	if (argc <= 1) {
		return 1;
	}
	
	//最大值
	unsigned int maxNum = 0;
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
		maxNum = (unsigned int) atol(argv[1]); // NOLINT(cert-err34-c)
	}
	
	int output = 1;
	if (argc > 2) {
		output = atoi(argv[2]); // NOLINT(cert-err34-c)
	}
	
	//分配内存，获得缓冲区;
	unsigned char *buff = malloc(neededSize(maxNum));
	
	//进行计算
	long t1 = getCurrentTime();
	goGetPrimeNumbers(maxNum, buff);
	long t2 = getCurrentTime();
	//输出
	if (output) {
		printf(maxNum < 100001 ? "2 " : "2\n");
		for (int i = 3; i <= maxNum; i += 2) {
			if (buff[i >> 4] & (1 << ((i >> 1) & 7))) {
				printf(maxNum < 100001 ? "%d " : "%d\n", i);
			}
		}
	}
	printf("\n%u numbers\nusing time:%li us, %li ms\n",
	       getPrimeCount(buff, neededSize(maxNum)),
	       t2 - t1, (t2 - t1) / 1000);
	free(buff);
	return 0;
}
