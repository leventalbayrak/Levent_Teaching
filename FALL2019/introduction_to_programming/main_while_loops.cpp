#include <stdio.h>



int main()
{
	//for loop
	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", i);
	}

	//while loop
	int x = 0;
	while (x < 10)
	{
		printf("%d\n", x);
		x++;
	}

	//do while loop
	int y = 0;
	do
	{
		printf("%d\n", y);
		y++;
	} while (y < 10);

	getchar();
	return 0;
}