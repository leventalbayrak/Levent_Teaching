#include <stdio.h>
#include <stdlib.h>

namespace Stack
{
	struct Stack
	{
		int n_data;
		int array_size;
		int *data;
	};
}

namespace Stack
{

	void init(Stack *s, int array_size)
	{
		s->n_data = 0;
		s->array_size = array_size;
		s->data = (int*)malloc(sizeof(int)*array_size);
	}

	void push(Stack *s, int value)
	{
		s->data[s->n_data++] = value;
	}

	int pop(Stack *s)
	{
		return s->data[--s->n_data];
	}

	void print(Stack *s)
	{
		for (int i = 0; i < s->n_data; i++)
		{
			printf("%d ", s->data[i]);
		}
		printf("\n");
	}
}

int main()
{
	Stack::Stack s;

	Stack::init(&s, 100);

	for (int i = 0; i < 10; i++)
	{
		Stack::push(&s, i*2);
	}
	Stack::print(&s);
	

	for (int i = 0; i < 2; i++)
	{
		int k = Stack::pop(&s);
	}
	Stack::print(&s);

	for (int i = 0; i < 5; i++)
	{
		Stack::push(&s, i * 2);
	}
	Stack::print(&s);

	for (int i = 0; i < 4; i++)
	{
		int k = Stack::pop(&s);
	}
	Stack::print(&s);

	getchar();
	return 0;
}