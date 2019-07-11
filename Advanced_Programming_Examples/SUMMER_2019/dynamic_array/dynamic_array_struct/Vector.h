#pragma once

struct Vector
{
	int n;
	int array_size;
	int *data;
};

void init(Vector *v)
{
	v->array_size = 100;
	v->data = (int*)malloc(sizeof(int)*v->array_size);
	v->n = 0;
}

void add(Vector *v, int what)
{
	if (v->n >= v->array_size)
	{
		v->array_size *= 1.2;
		int *tmp = (int*)malloc(sizeof(int)*v->array_size);
		memcpy(tmp, v->data, sizeof(int)*v->n);
		free(v->data);
		v->data = tmp;

		printf("array size %d\n", v->array_size);
	}
	v->data[v->n++] = what;
}
