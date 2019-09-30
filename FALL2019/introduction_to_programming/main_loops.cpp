#include <stdio.h>

int main()
{
	//option 1
	printf("*");
	printf("*");
	printf("*");
	printf("*");
	printf("*");
	printf("*");
	printf("\n");
	//option 2
	printf("******\n");
	//option 3
	for (int i = 0; i < 6; i++)
	{
		printf("*");
	}
	printf("\n");

	printf("BOX\n");
	//box
	for (int row = 0; row < 4; row++)
	{
		for (int i = 0; i < 5; i++)
		{
			printf("*");
		}
		printf("\n");
	}

	printf("BOX with variables\n");
	//box with variables
	int number_of_rows = 8;
	int number_of_stars_per_row = 60;

	for (int row = 0; row < number_of_rows; row++)
	{
		for (int i = 0; i < number_of_stars_per_row; i++)
		{
			printf("*");
		}
		printf("\n");
	}

	getchar();
	return 0;
}