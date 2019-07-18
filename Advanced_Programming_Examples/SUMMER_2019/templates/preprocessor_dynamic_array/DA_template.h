#pragma once

template <class T>
struct DA
{
	T* data;
	int size;
	int n;
};

template <class T>
void init(DA<T>* da)
{
	da->size = 1000;
	da->data = (TYPE*)malloc(sizeof(TYPE) * da->size);
	da->n = 0;
	
}

template <class T>
void add(DA<T>* da, T value)
{
	if (da->n >= da->size) return;
	da->data[da->n++] = value;
}