#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

namespace Queue
{
	struct Queue
	{
		int *data;
		int n_data;
		int array_size;
		int offset;
	};

	void init(Queue *s, int array_size)
	{
		s->n_data = 0;
		s->offset = 0;
		s->array_size = array_size;
		s->data = (int*)malloc(sizeof(int)*array_size);
	}

	void enqueue(Queue *s, int val)
	{
		int index = (s->offset + s->n_data) % s->array_size;
		s->data[index] = val;
		s->n_data++;
	}

	int dequeue(Queue *s)
	{
		int index = s->offset++;
		s->offset %= s->array_size;
		s->n_data--;
		return s->data[index];
	}

	void print(Queue *s)
	{
		for (int i = 0; i < s->n_data; i++)
		{
			printf("%d ", s->data[(s->offset + i) % s->array_size]);
		}
		printf("\n");
	}
}

int main()
{
	int array_size = 4;
	Queue::Queue q;
	Queue::init(&q, array_size);

	Queue::enqueue(&q, 5);
	Queue::enqueue(&q, 6);

	Queue::print(&q);

	int val = Queue::dequeue(&q);
	printf("dequeued %d\n", val);
	Queue::print(&q);

	getchar();
	return 0;
}