#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main()
{
	int data[100] = { 0 };
	int sum = 0;
#pragma omp parallel for num_threads(16) reduction(+:sum)
	for (int i = 0; i < 100; i++)
	{
		sum += data[i];
	}

	for (int i = 0; i < 100; i++)
	{
		printf("data[i] = %d\n", data[i]);
	}

	getchar();
	return 0;
}