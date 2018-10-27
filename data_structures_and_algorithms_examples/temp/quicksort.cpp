#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int partition(int *data, int left, int right)
{
	int current = left;
	for (int i = left; i < right; i++)
	{
		if (data[i] <= data[right])
		{
			//swap
			int tmp = data[i];
			data[i] = data[current];
			data[current] = tmp;

			current++;
		}
	}

	int tmp = data[right];
	data[right] = data[current];
	data[current] = tmp;

	int pivot_index = current;
	return pivot_index;
}

void quicksort(int *data, int left, int right)
{
	if (left >= right) return;
	
	int pivot_index = partition(data, left, right);

	quicksort(data, left, pivot_index - 1);

	quicksort(data, pivot_index + 1, right);
}


int main()
{
	srand(time(0));

	int n_data = 1000;
	int *data = new int[n_data];
	for (int i = 0; i < n_data; i++)
	{
		data[i] = n_data - i;
	}

	quicksort(data, 0, n_data-1);

	for (int i = 1; i < n_data; i++)
	{
		if (data[i] < data[i - 1])
		{
			printf("YOU ARE FIRED!\n");
			getchar();
		}
	}
	printf("YOU ARE NOT FIRED\n");
	getchar();
	return 0;
}




