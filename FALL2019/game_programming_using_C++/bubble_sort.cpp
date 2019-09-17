#include <stdio.h>

void bubble_Sort_Example_1()
{
	const int n_data = 8;
	int data[n_data] = { 1,5,2,0,6,3,2 ,2 };

	for (int i = 0; i < n_data - 1; ++i)
	{
		for (int j = i + 1; j < n_data; ++j)
		{
			if (data[j] < data[i])
			{
				int tmp = data[i];
				data[i] = data[j];
				data[j] = tmp;
			}
		}
	}

	for (int i = 0; i < n_data; ++i)
	{
		printf("%d ", data[i]);
	}
	printf("\n");
}

int main()
{
	
	bubble_Sort_Example_1();

	getchar();

	return 0;
}