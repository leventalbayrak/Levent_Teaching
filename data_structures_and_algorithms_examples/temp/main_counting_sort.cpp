#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void counting_Sort_V1(int *data, int n_data, int *freq, int freq_size)
{
	memset(freq, 0, sizeof(int)*freq_size);
	//count data
	for (int i = 0; i < n_data; i++)
	{
		freq[data[i]]++;
	}

	//re-populate
	int k = 0;
	for (int i = 0; i < freq_size; i++)
	{
		for (int j = 0; j < freq[i]; j++)
		{
			data[k++] = i;
		}
	}
}

void counting_Sort_V2(int *sorted_data, int *data, int n_data, int *freq, int freq_size)
{
	//this is the implementation that you would find online.
	//the example data array here is an integer, therefore the frequency summation step is redundant. 
	//however, in practice,
	//you would use an extra auxiliary data array,
	//or an array of structs for this implementation

	memset(freq, 0, sizeof(int)*freq_size);
	//count data
	for (int i = 0; i < n_data; i++)
	{
		freq[data[i]]++;
	}

	//calculate running sum
	for (int i = 1; i < freq_size; i++)
	{
		freq[i] += freq[i - 1];
	}

	//re-populate
	for (int i = 0; i < n_data; i++)
	{
		//NOTE:
		//for a slightly slower but stable version, use 'n_data-i-1' instead of 'i'
		//in this particular function, stability is not an issue, however,
		//if the data was an array of structs or a parallel auxiliary data array was used,
		//stability COULD matter
		//you must reason about the data and stability requirements before making a decision
		int index = --freq[data[i]];
		sorted_data[index] = data[i];
	}

}


void counting_Sort_V3(int *sorted_index, int *data, int n_data, int *freq, int freq_size)
{
	//this implementation is very flexible when used along with auxiliary data

	memset(freq, 0, sizeof(int)*freq_size);

	for (int i = 0; i < n_data; i++)
	{
		freq[data[i]]++;
	}

	for (int i = 1; i < freq_size; i++)
	{
		freq[i] += freq[i - 1];
	}

	//'n_data - i -1' makes the sort stable. use 'i' otherwise.
	for (int i = 0; i < n_data; i++)
	{
		int index = n_data - i - 1;
		sorted_index[--freq[data[index]]] = index;	
	}
}


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