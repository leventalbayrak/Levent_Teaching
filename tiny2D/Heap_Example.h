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
		float *priority;
		int *data;

		int n_data;
		int array_size;
	};

	void init(Min_Heap *h, int size)
	{
		h->n_data = 1;
		h->array_size = size;
		h->priority = (float*)malloc(sizeof(float)*h->array_size);
		h->data = (int*)malloc(sizeof(int)*h->array_size);
	}

	void resize(Min_Heap *h)
	{
		h->array_size <<= 1;
		h->priority = (float*)realloc(h->priority, sizeof(float)*h->array_size);
		h->data = (int*)realloc(h->data, sizeof(int)*h->array_size);
	}

	void add(Min_Heap *h, float priority, int data)
	{
		if (h->n_data >= h->array_size) resize(h);

		int current_index = h->n_data++;
		h->priority[current_index] = priority;
		h->data[current_index] = data;

		for (;;)
		{
			if (current_index <= 1) break;
			int parent_index = current_index >> 1;
			if (h->priority[current_index] >= h->priority[parent_index]) break;

			float tmp_p = h->priority[current_index];
			h->priority[current_index] = h->priority[parent_index];
			h->priority[parent_index] = tmp_p;

			int tmp_d = h->data[current_index];
			h->data[current_index] = h->data[parent_index];
			h->data[parent_index] = tmp_d;

			current_index = parent_index;
		}
	}

	void remove(float &priority, int &data, Min_Heap *h)
	{
		priority = h->priority[1];
		data = h->data[1];

		--h->n_data;
		h->priority[1] = h->priority[h->n_data];
		h->data[1] = h->data[h->n_data];
		
		int current_index = 1;

		for (;;)
		{

			int child_index = current_index << 1;
			if (child_index >= h->n_data) break;

			if (child_index + 1 < h->n_data && h->priority[child_index] >= h->priority[child_index + 1])
			{
				child_index++;
			}

			if (h->priority[current_index] <= h->priority[child_index]) break;

			float tmp_p = h->priority[current_index];
			h->priority[current_index] = h->priority[child_index];
			h->priority[child_index] = tmp_p;

			int tmp_d = h->data[current_index];
			h->data[current_index] = h->data[child_index];
			h->data[child_index] = tmp_d;

			current_index = child_index;

		}
	}
}