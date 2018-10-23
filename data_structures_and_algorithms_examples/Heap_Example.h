/*
Data structures and algorithms
Levent Albayrak 2018
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
namespace Min_Heap
{
	struct Min_Heap
	{
		int *priority;
		int *data;

		int n_data;
		int array_size;
	};

	void init(Min_Heap *h, int size)
	{
		h->n_data = 0;
		h->array_size = size;
		h->priority = (int*)malloc(sizeof(int)*h->array_size);
		h->data = (int*)malloc(sizeof(int)*h->array_size);
	}

	void resize(Min_Heap *h)
	{
		h->array_size <<= 1;
		h->priority = (int*)realloc(h->priority, sizeof(int)*h->array_size);
		h->data = (int*)realloc(h->data, sizeof(int)*h->array_size);
	}

	void add(Min_Heap *h, int priority, int data)
	{
		if (h->n_data >= h->array_size) resize(h);

		int current_index = h->n_data++;
		h->priority[current_index] = priority;
		h->data[current_index] = data;

		for (;;)
		{
			if (current_index <= 0) break;
			int parent_index = (current_index - 1) / 2;
			if (h->priority[current_index] >= h->priority[parent_index]) break;

			int tmp_p = h->priority[current_index];
			h->priority[current_index] = h->priority[parent_index];
			h->priority[parent_index] = tmp_p;

			int tmp_d = h->data[current_index];
			h->data[current_index] = h->data[parent_index];
			h->data[parent_index] = tmp_d;

			current_index = parent_index;
		}
	}

	void remove(int &priority, int &data, Min_Heap *h)
	{
		priority = h->priority[0];
		data = h->data[0];

		h->priority[0] = h->priority[h->n_data - 1];
		h->data[0] = h->data[h->n_data - 1];
		h->n_data--;
		int current_index = 0;

		for (;;)
		{

			int child_index = 2 * current_index + 1;
			if (child_index >= h->n_data) break;

			if (child_index + 1 < h->n_data && h->priority[child_index] >= h->priority[child_index + 1])
			{
				child_index++;
			}

			if (h->priority[current_index] <= h->priority[child_index]) break;

			int tmp_p = h->priority[current_index];
			h->priority[current_index] = h->priority[child_index];
			h->priority[child_index] = tmp_p;

			int tmp_d = h->data[current_index];
			h->data[current_index] = h->data[child_index];
			h->data[child_index] = tmp_d;

			current_index = child_index;

		}
	}
}