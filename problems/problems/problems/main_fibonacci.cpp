#pragma warning(disable:4996)
#include <stdio.h>

int fib_Recursive(int n)
{
	if (n <= 2) return 1;
	return fib_Recursive(n - 1) + fib_Recursive(n - 2);
}

int fib_Memoized(int n, int *fm)
{
	if (n <= 2) return 1;
	if (fm[n] != -1) return fm[n];
	return fib_Memoized(n - 1, fm) + fib_Memoized(n - 2, fm);
}

int fib_Dynamic(int n)
{
	int fm[2];
	fm[0] = 0; fm[1] = 1;
	int ret = 0;
	for (int i = 2; i <= n; i++)
	{
		ret = fm[0] + fm[1];
		fm[0] = fm[1];
		fm[1] = ret;
	}
	return ret;
}

int main()
{
	int n = 6;

	int v0 = fib_Recursive(n);
	printf("recursive %d\n", v0);

	int fm[1000];
	for (int i = 0; i < 1000; i++) fm[i] = -1;
	int v1 = fib_Memoized(n, fm);
	printf("memoized %d\n", v1);

	int v2 = fib_Dynamic(n);
	printf("dynamic %d\n", v2);

	getchar();
	return 0;
}