#include <stdio.h>

void function1()
{
	int number_of_rows = 5;
	int number_of_columns = 8;

	int number_spaces = number_of_columns - 2;
	
	for (int i = 0; i < number_of_columns; i++)
	{
		printf("*");
	}
	printf("\n");

	for (int i = 0; i < number_of_rows - 2; i++)
	{
		printf("*");
		for (int j = 0; j < number_spaces; j++)
		{
			printf(" ");
		}
		printf("*\n");
	}

	for (int i = 0; i < number_of_columns; i++)
	{
		printf("*");
	}
	printf("\n");

}

//HOW TO
void make_a_sandwich()
{
	int number_of_rows = 5;
	for (int row = 0; row < number_of_rows; row++)
	{
		int number_of_columns = row + 1;
		for (int i = 0; i < number_of_columns; i++)
		{
			printf("*");
		}
		printf("\n");
	}
}

void function3()
{
	int number_of_rows = 6;
	for (int row = 0; row < number_of_rows; row++)
	{
		int number_of_stars = row + 1;
		int number_of_spaces = number_of_rows - number_of_stars;

		for (int i = 0; i < number_of_spaces; i++)
		{
			printf(" ");
		}
		for (int i = 0; i < number_of_stars; i++)
		{
			printf("*");
		}
		printf("\n");
	}
}

void function4()
{
	int number_of_rows = 5;
	int number_of_spaces = number_of_rows - 1;
	int number_of_stars = 1;
	for (int row = 0; row < number_of_rows; row++)
	{
		for (int i = 0; i < number_of_spaces; i++)
		{
			printf(" ");
		}
		for (int i = 0; i < number_of_stars; i++)
		{
			printf("*");
		}
		printf("\n");

		number_of_spaces--;
		number_of_stars += 2;

	}

}

int main()
{

	function1();
	make_a_sandwich();//MAKE IT
	function3();
	function4();

	getchar();
	return 0;
}