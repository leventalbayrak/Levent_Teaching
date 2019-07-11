#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

#include "Vector.h"

int main()
{
	Vector vector;
	init(&vector);

	for (int i = 0; i < 1000000; i++)
	{
		add(&vector, rand() % 100);
	}

	getchar();
	return 0;
}