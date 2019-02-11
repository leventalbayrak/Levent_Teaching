#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

const unsigned int max_array_size = 100000;

void print_Array(const float *data, unsigned int n_count)
{
	for (unsigned int i = 0; i < n_count; i++)
	{
		printf("%f\n", data[i]);
	}
}

void read_File(float *data, unsigned int *n_count, 
	unsigned int array_size, const char *filename)
{
	*n_count = 0;
	FILE *f = fopen(filename, "r");
	unsigned int k = 0;
	for (unsigned int i = 0; i < array_size; i++)
	{
		int r = fscanf(f, "%f", &data[k]);
		if (r != 1) break;
		k++;
	}
	*n_count = k;
	fclose(f);
}

void copy_Array(float *dest, const float *src, unsigned int n_count)
{
	for (unsigned int i = 0; i < n_count; i++)
	{
		dest[i] = src[i];
	}
}

void copy_Array_Reverse(float *dest, const float *src, unsigned int n_count)
{
	for (unsigned int i = 0; i < n_count; i++)
	{
		dest[i] = src[n_count-i-1];
	}
}

void copy_Range(float *dest, unsigned int *dest_n_count, 
	const float *src, unsigned int src_n_count, float min_val,float max_val)
{
	unsigned int k = 0;
	for (unsigned int i = 0; i < src_n_count; i++)
	{
		if (src[i] >= min_val && src[i] <= max_val)
		{
			dest[k++] = src[i];
		}
	}

	*dest_n_count = k;
}


int main()
{
	float *data_original = (float*)malloc(sizeof(float)*max_array_size);
	float *data_copy = (float*)malloc(sizeof(float)*max_array_size);
	float *data_reverse = (float*)malloc(sizeof(float)*max_array_size);
	float *data_range = (float*)malloc(sizeof(float)*max_array_size);

	unsigned int n_original_count = 0;
	unsigned int n_range_count = 0;

	read_File(data_original, &n_original_count, max_array_size, "data.txt");
	
	copy_Array(data_copy, data_original, n_original_count);

	copy_Array_Reverse(data_reverse, data_original, n_original_count);

	copy_Range(data_range, &n_range_count, 
		data_original, n_original_count, 2, 5);

	print_Array(data_original, n_original_count);
	printf("---\n");
	print_Array(data_copy, n_original_count);
	printf("---\n");
	print_Array(data_reverse, n_original_count);
	printf("---\n");
	print_Array(data_range, n_range_count);

	getchar();
	return 0;
}