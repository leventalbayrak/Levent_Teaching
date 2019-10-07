#include <stdio.h>



int main()
{
	//for loop
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			printf("%d, %d\n", i, j);
		}
	}

	//while loop
	int x = 0;
	while (x < 2)
	{
		int y = 0;
		while (y < 2)
		{
			printf("%d, %d\n", x, y);
			y++;
		}	
		x++;
	}

	//do while loop
	int a = 0;
	do
	{
		int b = 0;
		do
		{
			printf("%d, %d\n", a, b);
			b++;
		} while (b < 2);
		a++;
	} while (a < 2);

	getchar();
	return 0;
}