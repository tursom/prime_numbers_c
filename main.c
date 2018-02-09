#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int getPrimeNumbers(unsigned int maxnum, unsigned int *buff, unsigned int buffsize) {
	unsigned int sqmax = (unsigned int) sqrt(maxnum);
	unsigned int numbers = 1;
	buff[0] = 2;
	int flag = 1;
	for (unsigned int i = 3; i <= maxnum; i += 2) {
		unsigned int sqi = (unsigned int) sqrt(i);
		for (unsigned int n = 0; buff[n] <= sqi; n++)
			if (i % buff[n] == 0)
				flag = 0;
#ifdef _DEBUG_
		if (!(i % 10000000))
			printf("%u\n", i);
#endif
		if (flag) {
			buff[numbers] = i;
			numbers++;
			if (numbers == buffsize)
				break;
		}
		flag = 1;
	}
	return numbers;
}

int main(int argc, char *argv[]) {
	if (argc <= 1)
		return 1;

	unsigned int maxnum = 0;
	if (argc > 1) {
		if (atol(argv[1]) < 2)
			return 2;
		maxnum = (unsigned int) atol(argv[1]);
	}

	int output = 1;
	if (argc > 2) {
		output = atoi(argv[2]);
	}

	unsigned int buffsize = (unsigned int) (maxnum / log(maxnum) * (maxnum > 200000 ? 1.1 : 1.3));
#ifdef _DEBUG_
	printf("buff size:%u\n", buffsize);
#endif
	unsigned int *buff = malloc(sizeof(unsigned int) * buffsize);
	unsigned int numbers = getPrimeNumbers(maxnum, buff, buffsize);
	if (output)
		for (int i = 0; i < numbers; i++)
			printf("%u\n", buff[i]);
	printf("%u numbers\n", numbers);
	return 0;
}