#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

typedef unsigned char uchar;
int main()
{
	const int n = 5;
	uchar data[n];
	for (int i = 0; i < n; ++i) data[i] = i;
	for (int i = 0; i < n; ++i) printf("%d\n", (int)data[i]);
	for (int i = 0; i < n << 3; ++i)
	{
		int index = i >> 3;//divide by 8
		uchar bit = i & 7;//modulus with 8

		if (bit == 0) printf("----\n");

		if (data[index] & (1<<bit))
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}


	getchar();
	return 0;
}