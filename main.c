#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


size_t getPrimeNumber(unsigned int maxnum) {
	//获得合适的缓冲区大小用到的比例系数
	//分阶定义以优化内存占用
	static unsigned int buffDoff = 200000;
	static double bufSize1 = 1.1;
	static double bufSize2 = 1.3;
	//质数数目增长的近似函数
	return (size_t) (maxnum / log(maxnum)
	                 //乘以比例系数
	                 * (
			                 //优化内存占用
			                 maxnum > buffDoff ? bufSize1 : bufSize2
	                 ));
}

/*
 * 获取某个最大值一下的所有质数
 * maxnum:最大值
 * buff:缓冲区，计算结果就存在这个缓冲区里
 * buffsize:缓冲区大小，使用完缓冲区之后函数会立即返回
 * 返回值为计算出了多少个质数
 */
unsigned int getPrimeNumbers(
		unsigned int maxNumber,
		unsigned int *buff,
		size_t bufferSize) {
	//放入初始值
	unsigned int calcedPrimeNumbers = 1;
	buff[0] = 2;
	
	//对当前验证的数字进行开方处理，可将时间增长从n^2降为n*ln(n)
	unsigned int sqi = 1;
	//对从3到给定最大值之间的奇数依次进行验证计算
	for (unsigned int checkingNumber = 3; checkingNumber <= maxNumber; checkingNumber += 2) {
		if (checkingNumber >= sqi * sqi) sqi++;
		for (unsigned int n = 0; buff[n] < sqi; n++) {
			//判断是否整除
			if (checkingNumber % buff[n] == 0) {
				goto endOfForLoop;
			}
		}
		//记录当前数字
		buff[calcedPrimeNumbers] = checkingNumber;
		//计算出的质数数量+1
		calcedPrimeNumbers++;
		//如果用完了缓冲区，立即结束函数
		if (calcedPrimeNumbers == bufferSize) {
			return calcedPrimeNumbers;
		}
		endOfForLoop:;
	}
	
	//返回一共计算出了多少质数
	return calcedPrimeNumbers;
}

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

/*
 * 程序入口函数
 */
int main(int argc, char *argv[]) {
	//必须给定最大值
	if (argc <= 1) {
		return 1;
	}
	
	//最大值
	unsigned int maxnum = 0;
	if (argc > 1) {
		//验证maxnum是否小于2，小于2的话计算没有意义
		//这里不能用maxnum验证，有负数的问题
		if (atol(argv[1]) < 2) { // NOLINT(cert-err34-c)
			return 2;
		}
		
		//判断最大值是否溢出
		if (willOverFlow(argv[1])) {
			//如果溢出
			//输出错误提示
			printf("too large maxmiun number");
			//程序退出
			//返回1表示最大值溢出
			return 1;
		}
		
		//如果都没有问题
		maxnum = (unsigned int) atol(argv[1]); // NOLINT(cert-err34-c)
	}
	
	int output = 1;
	if (argc > 2) {
		output = atoi(argv[2]); // NOLINT(cert-err34-c)
	}
	
	//计算需要的缓冲区的大小
	size_t buffsize = getPrimeNumber(maxnum);
	
	//分配内存，获得缓冲区
	unsigned int *buff = malloc(sizeof(*buff) * buffsize);
	
	//进行计算
	unsigned int numbers = getPrimeNumbers(maxnum, buff, buffsize);
	
	//输出
	if (output) {
		for (int i = 0; i < numbers; i++) {
			printf("%u ", buff[i]);
		}
	}
	printf("\n%u numbers\n", numbers);
	
	return 0;
}
