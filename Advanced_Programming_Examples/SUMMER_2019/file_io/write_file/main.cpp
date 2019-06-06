#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("usage:\n");
		printf("./exe n_numbers filename\n");
		return 0;
	}

	printf("number of program arguments: %d\n", argc);
	for (int i = 0; i < argc; i++)
	{
		printf("argument[%d] = %s\n", i, argv[i]);
	}

	printf("seeding RNG...\n");
	srand(time(0));
	int n_numbers = atoi(argv[1]);
	const char* filename = argv[2];

	printf("opening file...\n");
	FILE *f = fopen(filename, "w");

	printf("hacking...\n");

	fprintf(f, "%d\n", n_numbers);
	for (int i = 0; i < n_numbers; i++)
	{
		int number = rand();
		fprintf(f, "%d\n", number);
	}

	fclose(f);
	printf("hacking complete...\a");

	return 0;
}