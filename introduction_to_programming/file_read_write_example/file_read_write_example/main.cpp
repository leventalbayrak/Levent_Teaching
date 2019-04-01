/*
Write a program that reads in a list of integers from a file
into an array, and prints out the largest value in the array.
You must create separate functions for loading the list,
and finding the maximum.
the first value in the file is the number of values following it.
there will not be more than 10000 values in the file.
*/

#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

int load_File(int* values, const char *filename)
{
	FILE *f = fopen(filename, "r");
	int number_of_values = 0;
	fscanf(f, "%d", &number_of_values);
	for (int i = 0; i < number_of_values; i++)
	{
		int temporary_value = 0;
		fscanf(f, "%d", &temporary_value);
		values[i] = temporary_value;
	}
	fclose(f);
	return number_of_values;
}

int find_Max(int* values, int number_of_values)
{
	int guess = values[0];
	for (int i = 1; i < number_of_values; i++)
	{
		if (values[i] > guess)
		{
			guess = values[i];
		}
	}
	return guess;
}

int main()
{

	int *values = (int*)malloc(sizeof(int) * 10000);
	int number_of_values = load_File(values, "data.txt");
	
	int largest_value = find_Max(values, number_of_values);
	printf("maximum value is: %d\n", largest_value);

	getchar();
	return 0;
}