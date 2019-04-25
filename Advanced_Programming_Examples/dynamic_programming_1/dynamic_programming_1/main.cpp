#pragma warning(disable:4996)
#include <stdio.h>


void print_Binary(int k, int n)
{
	static int bits[32];
	if (k >= n)
	{
		for (int i = 0; i < k; i++)
		{
			printf("%d", bits[i]);
		}
		printf("\n");
		return;
	}
	
	bits[k] = 0;
	print_Binary(k + 1, n);
	bits[k] = 1;
	print_Binary(k + 1, n);
}

int main()
{

	print_Binary(0, 20);
	getchar();
	return 0;
}