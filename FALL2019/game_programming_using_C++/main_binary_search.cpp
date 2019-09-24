
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#pragma warning(disable:4996)

int bsearch(int value, const int* data, int ndata)
{
	int L = 0;
	int R = ndata - 1;

	for (; L + 1 < R;)
	{
		int M = (L + R) / 2;
		if (value < data[M]) R = M;
		else L = M;
	}

	if (value == data[L]) return L;
	if (value == data[R]) return R;

	return 0;
}

long long bsearch_Ext(long long value, const long long* data, long long ndata, long long *c)
{
	long long L = 0;
	long long R = ndata - 1;
	long long count = 0;
	for (; L + 1 < R;)
	{
		long long M = (L + R) / 2;
		if (value < data[M]) R = M;
		else L = M;
		++count;
	}

	*c = count;

	if (value == data[L]) return L;
	if (value == data[R]) return R;

	return 0;
}

int test0()
{

	const int data[5] = { 1,2,3,4,5 };
	int ndata = 5;

	for (;;)
	{
		printf("value:");
		int value = 0;
		scanf("%d", &value);
		int r = bsearch(value, data, ndata);
		if (value == data[r])
		{
			printf("found at index %d\n", r);
		}
		else
		{
			printf("not found!\n");
		}
	}
	return 0;
}


int main()
{
	
	test0();

	return 0;
}