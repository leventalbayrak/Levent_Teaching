#include <stdio.h>
#pragma warning(disable:4996)


int main()
{
	const char* filename = "prices.txt";
	
	FILE* f = fopen(filename, "r");
	int nitems = 0;
	for (;;)
	{
		char tmp[64];
		float price = 0.0f;
		int r = fscanf(f, "%s %f", tmp, &price);
		if (r != 2) break;
		
		printf("%s %f\n", tmp, price);
	}

	getchar();
	return 0;
}