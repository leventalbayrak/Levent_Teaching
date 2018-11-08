#pragma once

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

