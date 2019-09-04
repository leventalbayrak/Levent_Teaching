#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

namespace Stack
{
	struct Stack
	{
		int n_data;
		int array_size;
		char *data;
	};
}

namespace Stack
{

	void init(Stack *s, int array_size)
	{
		s->n_data = 0;
		s->array_size = array_size;
		s->data = (char*)malloc(sizeof(char)*array_size);
	}

	void push(Stack *s, char value)
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
			printf("%c ", s->data[i]);
		}
		printf("\n");
	}
}

void reverse(char *dest, const char* src, Stack::Stack *s)
{
	int len = strlen(src);
	int number_of_pushes = 0;
	for (int i = 0; i < len; i++)
	{
		if (s->n_data >= s->array_size) break;
		Stack::push(s, src[i]);
		number_of_pushes++;
	}

	for (int i = 0; i < number_of_pushes; i++)
	{
		dest[i] = Stack::pop(s);
	}
	dest[number_of_pushes] = 0;
}

int main()
{
	Stack::Stack s;

	Stack::init(&s, 100);

	char tmp[32];
	reverse(tmp, "Levent", &s);
	
	printf("%s\n", tmp);
	
	getchar();
	return 0;
}