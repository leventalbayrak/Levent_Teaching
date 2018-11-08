#include "CountingSort_Example.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void demo_Variant_1()
{
	printf("---\n");
	printf("demo variant 1\n");
	int freq_size = 256;
	int *freq = (int*)malloc(sizeof(int) * freq_size);

	int n_data = 10;
	int *data = (int*)malloc(sizeof(int)*n_data);


	for (int i = 0; i < n_data; i++) data[i] = rand() % 256;

	printf("before sort\n");
	for (int i = 0; i < n_data; i++)
	{
		printf("%d\n", data[i]);
	}

	counting_Sort_V1(data, n_data, freq, freq_size);

	printf("after sort\n");
	for (int i = 0; i < n_data; i++)
	{
		printf("%d\n", data[i]);
	}
	printf("---\n");
}

void demo_Variant_2()
{
	printf("---\n");
	printf("demo variant 2\n");
	int freq_size = 256;
	int *freq = (int*)malloc(sizeof(int) * freq_size);

	int n_data = 10;
	int *data = (int*)malloc(sizeof(int)*n_data);
	int *sorted_data = (int*)malloc(sizeof(int)*n_data);


	for (int i = 0; i < n_data; i++) data[i] = rand() % 256;

	printf("before sort\n");
	for (int i = 0; i < n_data; i++)
	{
		printf("%d\n", data[i]);
	}

	counting_Sort_V2(sorted_data, data, n_data, freq, freq_size);

	printf("after sort\n");
	for (int i = 0; i < n_data; i++)
	{
		printf("%d\n", sorted_data[i]);
	}
	printf("---\n");
}


void demo_Variant_3()
{
	printf("---\n");
	printf("demo variant 3\n");

	int freq_size = 256;
	int *freq = (int*)malloc(sizeof(int) * freq_size);

	int n_data = 10;
	int *data = (int*)malloc(sizeof(int)*n_data);
	int *sorted_index = (int*)malloc(sizeof(int)*n_data);

	char **extra_data = (char**)malloc(sizeof(char*)*n_data);
	for (int i = 0; i < n_data; i++)
	{
		data[i] = rand() % 256;
		extra_data[i] = (char*)malloc(sizeof(char) * 32);
		int len = rand() % 32;
		for (int j = 0; j < len; j++) extra_data[i][j] = 'A' + rand() % 26;
		extra_data[i][len] = 0;
	}

	counting_Sort_V3(sorted_index, data, n_data, freq, freq_size);

	printf("before sort\n");
	for (int i = 0; i < n_data; i++)
	{
		printf("%3d -> %s\n", data[i], extra_data[i]);
	}

	printf("after sort\n");
	for (int i = 0; i < n_data; i++)
	{
		printf("%3d -> %s\n", data[sorted_index[i]], extra_data[sorted_index[i]]);
	}

	printf("---\n");
}

int main()
{
	srand(time(0));

	demo_Variant_1();
	demo_Variant_2();
	demo_Variant_3();

	getchar();
	return 0;
}
