#include "Quicksort_Example.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	srand(time(0));

	int n_data = 1000;
	int *data = new int[n_data];
	for (int i = 0; i < n_data; i++)
	{
		data[i] = n_data - i;
	}

	quicksort(data, 0, n_data - 1);

	for (int i = 1; i < n_data; i++)
	{
		if (data[i] < data[i - 1])
		{
			printf("YOU ARE FIRED!\n");
			getchar();
		}
	}
	printf("YOU ARE NOT FIRED\n");
	getchar();
	return 0;
}