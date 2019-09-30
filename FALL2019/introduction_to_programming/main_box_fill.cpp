#include <stdio.h>
int main()
{
	//box
	for (int row = 0; row < 4; row++)
	{
		for (int i = 0; i < 5; i++)
		{
			printf("*");
		}
		printf("\n");
	}
	getchar();
	return 0;
}