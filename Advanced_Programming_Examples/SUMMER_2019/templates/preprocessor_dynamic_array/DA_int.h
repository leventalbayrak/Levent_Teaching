#pragma once

#define TYPE int
#define STRUCT DA_int

struct STRUCT
{
	TYPE* data;
	int size;
	int n;
};

void init(DA_int* da)
{
	da->size = 1000;
	da->data = (TYPE*)malloc(sizeof(TYPE) * da->size);
	da->n = 0;
	
}

void add(DA_int* da, TYPE value)
{
	if (da->n >= da->size) return;
	da->data[da->n++] = value;
}