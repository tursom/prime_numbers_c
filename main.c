#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

int goGetPrimeNumbers(unsigned int maxNumber, char *buffer) {
	if (maxNumber < 2)return 0;
	int primeNumberCount = 1;
	for (int k = 0; k < neededSize(maxNumber); ++k) {
		buffer[k] = (char) 0xff;
	}
	unsigned int sqrtMaxNumber = (unsigned int) sqrt(maxNumber);
	for (size_t i = 3; i <= sqrtMaxNumber; i += 2) {
		if (buffer[i >> 4] & 1 << (i >> 1) % 8) {
			++primeNumberCount;
			for (size_t j = i + i + i; j <= maxNumber; j += i + i) {
				buffer[j >> 4] &= (char) ~(1 << (j >> 1) % 8);
			}
		}
	}
	return primeNumberCount;
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
	char *buff = malloc(neededSize(maxNum));
	
	//进行计算
	goGetPrimeNumbers(maxNum, buff);
	//输出
	if (output) {
		printf(maxNum < 100001 ? "2 " : "2\n");
		for (int i = 3; i <= maxNum; i += 2) {
			if (buff[i / 16] & (1 << ((i / 2) % 8))) {
				printf(maxNum < 100001 ? "%d " : "%d\n", i);
			}
		}
	}
	int numbers = 1;
	for (int i = 3; i <= maxNum; i += 2) {
		if (buff[i / 16] & (1 << ((i / 2) % 8))) {
			numbers++;
		}
	}
	printf("\n%u numbers\n", numbers);
	free(buff);
	return 0;
}
