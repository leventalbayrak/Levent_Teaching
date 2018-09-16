#pragma once
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable:4996)

namespace Queue
{
	struct Queue
	{
		int *data;
		int n_data;
		int size;
		int offset;
	};

	void resize(Queue *c)
	{
		int new_size = c->size + (c->size >> 1);
		int *tmp = (int*)malloc(sizeof(int)*new_size); assert(tmp);

		int n_elements_before_offset = (c->offset + c->n_data) % c->size;
		int n_elements_after_offset = c->n_data - n_elements_before_offset;
		
		memcpy(tmp, &c->data[c->offset], sizeof(int)*n_elements_after_offset);
		memcpy(&tmp[n_elements_after_offset], c->data, sizeof(int)*n_elements_before_offset);

		c->offset = 0;
		c->size = new_size;
		free(c->data);
		c->data = tmp;
	}

	void init(Queue* c, unsigned int initial_size)
	{
		assert(initial_size != 0);
		c->n_data = 0;
		c->size = initial_size;
		c->offset = 0;
		c->data = (int*)malloc(sizeof(int)*c->size); assert(c->data);
	}

	void enqueue(Queue *c, int val)
	{
		if (c->n_data >= c->size) resize(c);
		c->data[(c->offset + c->n_data) % c->size] = val;
		c->n_data++;
	}

	int dequeue(Queue *c)
	{
		int old_offset = c->offset;
		c->offset = (c->offset + 1) % c->size;
		c->n_data--;
		return c->data[old_offset];
	}

	void clear(Queue *c)
	{
		c->n_data = 0;
		c->offset = 0;
	}

	void cleanup(Queue *c)
	{
		free(c->data);
		*c = {};
	}
}