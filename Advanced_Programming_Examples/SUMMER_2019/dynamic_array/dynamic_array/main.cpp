#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)


int main()
{
	int array_size = 100;
	int *data = (int*)malloc(sizeof(int)*array_size);
	int n = 0;
	for (int i = 0; i < 1000000; i++)
	{
		if (n >= array_size)
		{
			array_size *= 1.2;
			int *tmp = (int*)malloc(sizeof(int)*array_size);
			memcpy(tmp, data, sizeof(int)*n);
			free(data);
			data = tmp;

			printf("new array size is %d\n", array_size);
		}
		data[n++] = rand() % 100;
	}

	getchar();
	return 0;
}