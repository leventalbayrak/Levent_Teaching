#include <stdio.h>

void counting_Sort_Example_1()
{
	const int freq_array_size = 101;//allows numbers 0 to 100
	const int n_data = 6;
	int data[n_data] = { 1,5,2,0,6,3 };
	int freq[freq_array_size] = { 0 };

	for (int i = 0; i < n_data; ++i)
	{
		freq[data[i]]++;
	}

	int k = 0;
	for (int i = 0; i < freq_array_size; i++)
	{
		for (int j = 0; j < freq[i]; ++j)
		{
			data[k++] = i;
		}
		if (k >= n_data) break;
	}

	for (int i = 0; i < n_data; ++i)
	{
		printf("%d ", data[i]);
	}
	printf("\n");
}

void counting_Sort_Example_2()
{
	const int freq_array_size = 101;//allows numbers 0 to 100
	const int n_data = 8;
	int data[n_data] = { 1,5,2,0,6,3,2,2 };
	const char *data_ext = "abcdefgh";
	int sorted_data[n_data] = { 0 };
	char sorted_data_ext[n_data] = { 0 };
	int freq[freq_array_size] = { 0 };

	for (int i = 0; i < n_data; ++i)
	{
		freq[data[i]]++;
	}

	for (int i = 1; i < freq_array_size; ++i)
	{
		freq[i] += freq[i - 1];
	}

	for (int i = n_data-1; i >= 0; --i)
	{
		int k = --freq[data[i]];
		sorted_data[k] = data[i];
		sorted_data_ext[k] = data_ext[i];
	}
	
	for (int i = 0; i < n_data; ++i)
	{
		printf("%c %d\n",sorted_data_ext[i], sorted_data[i]);
	}
	printf("\n");
}


int main()
{
	
	counting_Sort_Example_2();

	getchar();

	return 0;
}