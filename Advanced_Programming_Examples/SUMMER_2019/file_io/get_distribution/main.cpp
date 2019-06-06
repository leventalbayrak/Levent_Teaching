#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("usage:\n");
		printf("./exe input_filename output_filename\n");
		return 0;
	}

	printf("number of program arguments: %d\n", argc);
	for (int i = 0; i < argc; i++)
	{
		printf("argument[%d] = %s\n", i, argv[i]);
	}

	const char* filename = argv[1];

	printf("opening file...\n");
	FILE *f = fopen(filename, "r");
	if (f == NULL)
	{
		printf("file not found!\n");
		return 0;
	}

	int n_numbers = 0;
	fscanf(f, "%d", &n_numbers);
	printf("loading %d numbers...\nthis may take a while!\n", n_numbers);

	int *numbers = (int*)malloc(sizeof(int)*n_numbers);

	for (int i = 0; i < n_numbers; i++)
	{
		//fscanf(f, "%d", numbers + i);	
		fscanf(f, "%d", &numbers[i]);
	}
	fclose(f);

	printf("processing numbers...\n");

	int _min = numbers[0];
	int _max = _min;
	for (int i = 1; i < n_numbers; i++)
	{
		if (numbers[i] < _min) _min = numbers[i];
		if (numbers[i] > _max) _max = numbers[i];
	}

	printf("min: %d max: %d\n", _min, _max);

	int *frequency = (int*)malloc(sizeof(int)*(_max + 1));
	for (int i = 0; i < _max + 1; i++)
	{
		frequency[i] = 0;
	}

	for (int i = 0; i < n_numbers; i++)
	{
		frequency[numbers[i]]++;
	}

	const char* output_filename = argv[2];
	FILE *f_freq = fopen(output_filename, "w");
	for (int i = 0; i < _max + 1; i++)
	{
		fprintf(f_freq, "%d\t%d\n", i, frequency[i]);
	}
	fclose(f_freq);
	
	free(numbers);
	free(frequency);

	return 0;
}