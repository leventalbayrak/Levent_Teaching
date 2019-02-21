#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float calculate_Average(float *values, int number_of_elements)
{
	float sum = 0.0;
	for (int i = 0; i < number_of_elements; i++)
	{
		sum += values[i];
	}

	float avg = sum / number_of_elements;
	return avg;
}

float calculate_Stdev(float avg, float *data, int number_of_things)
{
	float sum = 0.0;
	for (int i = 0; i < number_of_things; i++)
	{
		float difference = data[i] - avg;
		sum += difference*difference;
	}

	float standard_deviation = sqrt(sum / number_of_things);
	return standard_deviation;
}

int main()
{
	FILE *f = fopen("data.txt", "r");
	if (f == NULL)
	{
		printf("file not found!\n");
		return 0;
	}

	int array_size = 1000000;
	float *data = (float*)malloc(sizeof(float)*array_size);
	if (data == NULL)
	{
		printf("not enough memory!\n");
		return 0;
	}

	int number_of_values = 0;
	for (int i = 0; i < array_size; i++)
	{
		float value = 0.0;
		int r = fscanf(f, "%f", &value);
		if (r != 1) break;

		data[number_of_values++] = value;
	}

	float avg = calculate_Average(data, number_of_values);
	float stdev = calculate_Stdev(avg, data, number_of_values);
	
	printf("the average is %f and the stdev is %f\n", avg, stdev);

	getchar();
	return 0;
}