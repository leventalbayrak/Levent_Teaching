#include "Heap_Example.h"
#include <Windows.h>
#include <time.h>
#include <iostream>
using namespace std;

int cmp_Int(const void *a, const void *b)
{
	return *((int*)a) - *((int*)b);
}

int main()
{
	Min_Heap::Min_Heap heap;
	Min_Heap::init(&heap, 1024);

	int n_data = 160090;
	int *sorted_data = new int[n_data];
	for (int i = 0; i < n_data; i++)
	{
		int priority = rand();
		sorted_data[i] = priority;

		Min_Heap::add(&heap, priority);
	}

	qsort(sorted_data, n_data, sizeof(int), cmp_Int);

	bool you_are_fired = false;
	for (int i = 0; i < n_data; i++)
	{
		int priority = Min_Heap::remove(&heap);
		if (sorted_data[i] != priority)
		{
			you_are_fired = true;
			cout << "mismatch at " << i << " " << sorted_data[i] << " " << priority << endl;
		}
	}

	if (you_are_fired)
	{
		cout << "please debug better" << endl;
	}
	else
	{
		cout << "good job" << endl;
	}
	getchar();
	return 0;
}