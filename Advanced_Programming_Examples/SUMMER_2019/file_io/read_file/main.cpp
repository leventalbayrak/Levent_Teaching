#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage:\n");
		printf("./exe filename\n");
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

	float average = 0.0;
	for (int i = 0; i < n_numbers; i++)
	{
		average += numbers[i];
	}
	average /= n_numbers;

	float sum_of_squared_differences = 0.0;
	for (int i = 0; i < n_numbers; i++)
	{
		float diff = numbers[i] - average;
		sum_of_squared_differences += diff * diff;
	}

	float variance = sum_of_squared_differences /n_numbers;
	float standard_deviation = sqrt(variance);

	printf("average: %f stdev: %f\n", average, standard_deviation);

	free(numbers);

	return 0;
}