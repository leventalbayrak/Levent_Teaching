#include <stdio.h>


int main()
{
	
	int temperature = 97;

	if (temperature > 72)
	{
		printf("it is hot!\n");
	}

	if (temperature < 65)
	{
		printf("turn the heat up!\n");
	}
	else
	{
		printf("don't do anything!\n");
	}

	if (temperature == 97)
	{
		printf("way too hot!\n");
	}

	if (temperature == 82)
	{
		printf("it's still hot\n");
	}

	getchar();
	return 0;
}