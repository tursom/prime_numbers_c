#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//调试
//#define _DEBUG_
//调试的时候进度显示的单位
#define _CHEK_NUM_ = 10000000

//优化内存占用
#define BUFF_DIFF 200000
#define BUFF1 1.1
#define BUFF2 1.3

unsigned int getPrimeNumbers(unsigned int maxnum, unsigned int *buff, unsigned int buffsize) {
	//放入初始值
	unsigned int numbers = 1;
	buff[0] = 2;

	//用来判定一个数是不是质数
	int flag = 1;
	//对从3到给定最大值之间的奇数依次进行验证计算
	for (unsigned int i = 3; i <= maxnum; i += 2) {
		//对当前验证的数字进行开方处理，可将时间增长从n^2降为n*ln(n)
		unsigned int sqi = (unsigned int) sqrt(i);
		//从第一个质数到需要验证的最大的质数
		for (unsigned int n = 0; buff[n] <= sqi; n++)
			//判断是否整除
			if (i % buff[n] == 0) {
				//如果整除，则当前数字不是质数
				flag = 0;
				//跳出循环
				break;
			}

		if (flag) {
			//如果没有把任何质数整除，则当前数字是质数
			//记录当前数字
			buff[numbers] = i;
			//计算出的质数数量+1
			numbers++;
			//如果用完了缓冲区，立即结束函数
			if (numbers == buffsize)
				return numbers;
		}
		//重置flag
		flag = 1;

#ifdef _DEBUG_
#ifdef _CHEK_NUM_
		if (!(i % _CHEK_NUM_))
			printf("%u\n", i);
#endif
#endif
	}

	return numbers;
}

/*
 * 判断字符串s1是否比s2小
 */
int str_smaller(char *s1, char *s2) {
	return *s2 == '-' || strlen(s1) < strlen(s2) || strcmp(s1, s2) < 0;
}

int main(int argc, char *argv[]) {
	if (argc <= 1)
		return 1;

	unsigned int maxnum = 0;
	if (argc > 1) {
		//验证maxnum是否小于2，小于2的话计算没有意义
		//这里不能用maxnum验证，有负数的问题
		if (atol(argv[1]) < 2)
			return 2;

		//判断最大值是否溢出
		char c[32] = {'\0'};
		unsigned int a = 1;
		sprintf(c, "%d", (a << (sizeof(a) * 8 - 1)) - 1);
		if (str_smaller(c, argv[1])) {
			//如果溢出
			//输出错误提示
			printf("too large maxmiun number");
			//程序退出
			//返回1表示最大值溢出
			return 1;
		}

		//如果都没有问题
		maxnum = (unsigned int) atol(argv[1]);
	}

	int output = 1;
	if (argc > 2) {
		output = atoi(argv[2]);
	}

	//计算需要的缓冲区的大小
	unsigned int buffsize = (unsigned int) (
			//质数数目增长的近似函数
			maxnum / log(maxnum)
			//比例系数
			* (
					//优化内存占用
					//BUFF_DIFF 200000
					//BUFF1 1.1
					//BUFF2 1.3
					maxnum > BUFF_DIFF ? BUFF1 : BUFF2
			));

#ifdef _DEBUG_
	//输出缓冲区大小，调试用
	printf("buff size:%u\n", buffsize);
#endif

	//分配内存，获得缓冲区
	unsigned int *buff = malloc(sizeof(unsigned int) * buffsize);

	//进行计算
	unsigned int numbers = getPrimeNumbers(maxnum, buff, buffsize);

	//输出
	if (output)
		for (int i = 0; i < numbers; i++)
			printf("%u\n", buff[i]);
	printf("%u numbers\n", numbers);

	return 0;
}