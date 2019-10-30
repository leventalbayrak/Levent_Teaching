#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

void print_Binary1(int number)
{
	for (int i = 31; i >= 1; --i)
	{
		int bit = number >> i;
		bit &= 1;
		printf("%d", bit);
	}
	printf("\n");
}

void print_Binary2(int number)
{
	int bits[32];

	int k = 0;
	for (;;)
	{
		bits[k++] = number % 2;
		number /= 2;

		if (number == 0) break;
	}

	for (int i = 0; i < k; i++)
	{
		printf("%d", bits[k - i - 1]);
	}
	printf("\n");
}

int main()
{

	print_Binary1(243);
	print_Binary2(243);

	getchar();
	return 0;
}