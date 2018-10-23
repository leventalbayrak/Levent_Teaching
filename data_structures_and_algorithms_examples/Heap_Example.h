/*
Data structures and algorithms
Levent Albayrak 2018
*/
#pragma once
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable:4996)

namespace Min_Heap
{
	struct Min_Heap
	{
		int *priority;
		int n_data;
		int size;
	};

	void init(Min_Heap* h, int size)
	{
		h->n_data = 0;
		h->size = size;
		h->priority = (int*)malloc(sizeof(int)*h->size); assert(h->priority);
	}

	void resize(Min_Heap *h)
	{
		h->size <<= 1;
		h->priority = (int*)realloc(h->priority, sizeof(int)*h->size); assert(h->priority);
	}

	void add(Min_Heap *h, int priority)
	{
		if (h->n_data >= h->size) resize(h);

		h->priority[h->n_data] = priority;

		int current_index = h->n_data;

		for (;;)
		{
			if (current_index <= 0) break;

			int parent_index = (current_index - 1) >> 1;
			if (h->priority[current_index] < h->priority[parent_index])
			{
				//swap child priority with parent priority
				int tmp = h->priority[current_index];
				h->priority[current_index] = h->priority[parent_index];
				h->priority[parent_index] = tmp;
				
				current_index = parent_index;
			}
			else
			{
				break;
			}
		}

		h->n_data++;
	}

	int remove(Min_Heap *h)
	{
		int root_priority = h->priority[0];

		h->priority[0] = h->priority[h->n_data - 1];

		int current_index = 0;

		for (;;)
		{
			if (current_index >= h->n_data) break;

			int left_child_index = current_index * 2 + 1;

			if (left_child_index >= h->n_data) break;

			int right_child_index = left_child_index + 1;

			int better_child_index;

			if (right_child_index >= h->n_data)
			{
				better_child_index = left_child_index;
			}
			else
			{
				better_child_index = left_child_index;
				if (h->priority[right_child_index] < h->priority[better_child_index])
				{
					better_child_index = right_child_index;
				}
			}

			if (h->priority[current_index] > h->priority[better_child_index])
			{
				//swap child priority with parent priority
				int tmp = h->priority[current_index];
				h->priority[current_index] = h->priority[better_child_index];
				h->priority[better_child_index] = tmp;

				current_index = better_child_index;
			}
			else
			{
				break;
			}
			
		}

		h->n_data--;
		return root_priority;
	}

}