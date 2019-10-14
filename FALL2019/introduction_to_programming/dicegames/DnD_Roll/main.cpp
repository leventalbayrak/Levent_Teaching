#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)

int main()
{
	printf("//-------DUNGEON MASTER 2.17--------\\\\\n");
	
	srand(time(0));

	for (;;)
	{
		printf(">");
		int how_many = 0;
		int value = 0;
		scanf("%d%d", &value, & how_many);

		for (int i = 0; i < how_many; ++i)
		{
			int roll = 1 + rand() % value;
			printf("%d\n", roll);
		}
	}

	getchar();
	return 0;
}