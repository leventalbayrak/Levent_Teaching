#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "Stopwatch.h"
using namespace std;

namespace Queue
{
	struct Queue
	{
		int *data;
		int n_data;
		int offset;
		int size;
	};
}

namespace Queue_Core
{
	namespace Internal
	{
		void resize(Queue::Queue *q)
		{
			q->size += q->size >> 1;
			q->data = (int*)realloc(q->data, sizeof(int)*q->size); assert(q->data);
			if (q->offset != 0) memcpy(&q->data[q->n_data], q->data, sizeof(int)*q->offset);
		}
	}
	void init(Queue::Queue *q, int initial_size)
	{
		q->n_data = 0;
		q->offset = 0;
		q->size = initial_size;
		q->data = (int*)malloc(sizeof(int)*q->size); assert(q->data);
	}
	void enqueue(Queue::Queue *q, int val)
	{
		if (q->n_data >= q->size) Internal::resize(q);

		q->data[(q->offset + q->n_data) % q->size] = val;
		q->n_data++;
	}
	int dequeue(Queue::Queue *q)
	{
		int old_offset = q->offset++;
		q->offset %= q->size;
		q->n_data--;
		return q->data[old_offset];
	}
}

namespace Queue_Core_Fast
{
	namespace Internal
	{
		void resize(Queue::Queue *q)
		{
			q->size <<= 1;
			q->data = (int*)realloc(q->data, sizeof(int)*q->size); assert(q->data);
			if (q->offset != 0) memcpy(&q->data[q->n_data], q->data, sizeof(int)*q->offset);
		}
	}
	void init(Queue::Queue *q, int initial_size)
	{
		assert((initial_size & (initial_size - 1)) == 0);
		q->n_data = 0;
		q->offset = 0;
		q->size = initial_size;
		q->data = (int*)malloc(sizeof(int)*q->size); assert(q->data);
	}
	void enqueue(Queue::Queue *q, int val)
	{
		if (q->n_data >= q->size) Internal::resize(q);

		q->data[(q->offset + q->n_data) & (q->size - 1)] = val;
		q->n_data++;
	}
	int dequeue(Queue::Queue *q)
	{
		int old_offset = q->offset++;
		q->offset &= q->size - 1;
		q->n_data--;
		return q->data[old_offset];
	}
}



int main()
{
	Stopwatch::init();

	int n_repeat = 1000000;
	int *n = new int[n_repeat];
	int *op = new int[n_repeat];
	for (int i = 0; i < n_repeat; i++)
	{
		n[i] = i;
		n[i] = (i % 6) == 0 ? 0 : 1;
	}

	Queue::Queue qA;
	Queue_Core::init(&qA, 8);

	Queue::Queue qB;
	Queue_Core_Fast::init(&qB, 8);

	Stopwatch::begin();
	for (int i = 0; i < n_repeat; i++)
	{
		if (op[i] == 0)
		{
			Queue_Core::enqueue(&qA, i);
		}
		else
		{
			n[i] = Queue_Core::dequeue(&qA);
		}
	}
	Stopwatch::end();
	
	Stopwatch::begin();
	for (int i = 0; i < n_repeat; i++)
	{
		if (op[i] == 0)
		{
			Queue_Core_Fast::enqueue(&qB, i);
		}
		else
		{
			n[i] = Queue_Core_Fast::dequeue(&qB);
		}
	}
	Stopwatch::end();

	Stopwatch::print();


	getchar();
}