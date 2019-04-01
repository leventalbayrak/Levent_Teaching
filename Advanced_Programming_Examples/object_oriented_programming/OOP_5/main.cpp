#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Goat.h"

int main()
{
	srand(time(0));

	Goat goat;

	for (;;)
	{
		goat.hunt();
		getchar();
	}

	return 0;
}