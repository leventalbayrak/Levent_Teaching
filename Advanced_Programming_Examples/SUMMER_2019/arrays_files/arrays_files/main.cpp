#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

const int MAX_SIZE = 1000000;
void read_File(const char* filename, float *data, int *n_data)
{
	FILE *f = fopen(filename, "r");
	*n_data = 0;
	int i = 0;
	for (;;)
	{
		float value = 0.0;
		int r = fscanf(f, "%f", &value);
		if (i >= MAX_SIZE || r != 1) break;
		data[i++] = value;
	}

	fclose(f);
	*n_data = i;
}

void print_Array(const float* data, int n_data)
{
	for (int i = 0; i < n_data; i++)
	{
		printf("%f\n", data[i]);
	}
}

void copy_Array(float *dest, const float *src, int n_data)
{
	for (int i = 0; i < n_data; i++)
	{
		dest[i] = src[i];
	}
}

void copy_Array_Reverse(float *dest, const float *src, int n_data)
{
	for (int i = 0; i < n_data; i++)
	{
		dest[i] = src[n_data - i - 1];
	}
}

void copy_Range(float *dest, int *n_dest, const float *src, int n_src, float _min, float _max)
{
	int k = 0;
	for (int i = 0; i < n_src; i++)
	{
		if (src[i] >= _min && src[i] <= _max)
		{
			dest[k++] = src[i];
		}
	}
	*n_dest = k;
}

int main()
{
	float *data = (float*)malloc(sizeof(float)*MAX_SIZE);
	int n_data = 0;
	read_File("floats.txt", data, &n_data);

	printf("original array\n");
	print_Array(data, n_data);

	float *copy_data = (float*)malloc(sizeof(float)*n_data);

	copy_Array(copy_data, data, n_data);
	printf("copied array\n");
	print_Array(copy_data, n_data);

	copy_Array_Reverse(copy_data, data, n_data);
	printf("reverse copied array\n");
	print_Array(copy_data, n_data);

	int n_range = 0;
	copy_Range(copy_data, &n_range, data, n_data, 0.2, 0.6);
	printf("ranged copied array\n");
	print_Array(copy_data, n_range);


	getchar();
	return 0;
}