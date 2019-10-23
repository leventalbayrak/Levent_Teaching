/*
QUIZ 3 Review
Write a program that prints the odd numbers between range "A" and "B" (inclusive),
followed by the even numbers in the same range.
- you must write two separate functions to perform the printing.
void print_Odd(int A, int B);
void print_Even(int A, int B);
*/

#include <stdio.h>
#pragma warning(disable:4996)

void print_Odd(int A, int B)
{
	for (int i = A; i <= B; ++i)
	{
		//% (modulus) gives you the remainder of an integer division
		if (i % 2 == 1)
		{
			printf("%d\n", i);
		}
	}
}

void print_Even(int A, int B)
{
	for (int i = A; i <= B; ++i)
	{
		//% (modulus) gives you the remainder of an integer division
		if (i % 2 == 0)
		{
			printf("%d\n", i);
		}
	}
}

int main()
{
	int A = 1;
	int B = 39;

	print_Odd(A, B);
	print_Even(A, B);

	getchar();
	return 0;
}