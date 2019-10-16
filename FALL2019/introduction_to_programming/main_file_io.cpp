#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

void generate_Data(const char* filename)
{
	FILE* f = fopen(filename, "w");
	for (int i = 0; i < 1000; ++i)
	{
		fprintf(f, "%d\n", i);
		printf("%d\n", i);
	}
	fclose(f);
}

void display_Data(const char* filename)
{
	FILE* f = fopen(filename, "r");
	for (;;)
	{
		int value = 0;
		int r = fscanf(f, "%d\n", &value);
		if (r != 1)
		{
			printf("end of file reached\n");
			break;
		}
		printf("%d\n", value);
	}

	fclose(f);
}

int load_Data(const char* filename, int* data)
{
	int number_of_items_read = 0;
	FILE* f = fopen(filename, "r");
	for (;;)
	{
		int value = 0;
		int r = fscanf(f, "%d\n", &value);

		if (r != 1)
		{
			break;
		}

		data[number_of_items_read] = value;
		number_of_items_read++;

	}

	fclose(f);

	return number_of_items_read;
}

int main()
{
	
	generate_Data("data.txt");
	printf("generated data\n");
	getchar();

	display_Data("data.txt");
	printf("displayed data\n");
	getchar();

	static int data[1000000];
	int N = load_Data("data.txt", data);

	for (int i = 0; i < N; ++i)
	{
		printf("%d\n", data[i]);
	}
	
	getchar();
	return 0;
}






