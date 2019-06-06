#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)


//returns 1 if the number 'n' is prime, 0 otherwise
int is_Prime(int n)
{
	for (int k = 2; k < n; k++)
	{
		if (n % k == 0) return 0;
	}
	return 1;
}

int main(int argc, char** argv)
{
	printf("welcome to Mark's prime finder!\n");
	if (argc != 4)
	{
		printf("usage:\n");
		printf("./exe filename begin end\n");
		return 0;
	}

	FILE *f = fopen(argv[1], "w");
	
	int begin = atoi(argv[2]);
	int end = atoi(argv[3]);

	if (begin < 2) begin = 2;

	if (begin == 2)
	{
		fprintf(f, "%d\n", 2);
	}

	//make sure begin is and odd number
	//if it is even, make it odd by 
	//going to the next number
	if (begin % 2 == 0) begin++;

	for (int n = begin; n <= end; n+=2)
	{
		if (is_Prime(n) == 1)
		{
			fprintf(f, "%d\n", n);
		}
	}

	fclose(f);

	return 0;
}