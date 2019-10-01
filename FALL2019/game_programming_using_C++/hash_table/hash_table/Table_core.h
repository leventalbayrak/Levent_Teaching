#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
#include "Table_data.h"

namespace Table
{

	void init(Table *t, int size)
	{
		t->size = size;
		t->keys = (long long*)calloc(size, sizeof(long long));
	}

	void insert(long long key, Table *t)
	{
		int index = key % t->size;
		int size = t->size;
		long long *p = t->keys;
		for (int i = 0; i < size; ++i)
		{
			if (p[index] == key)
			{
				break;
			}
			else if (p[index] == 0)
			{
				p[index] = key;
				break;
			}
			++index;
			index %= size;
		}
	}

	int search(long long key, Table *t)
	{
		int index = key % t->size;
		int size = t->size;
		long long *p = t->keys;
		for (int i = 0; i < size; ++i)
		{
			if (p[index] == key)
			{
				return 1;
			}
			else if (p[index] == 0)
			{
				return 0;
			}
			++index;
			index %= size;
		}

		return 0;
	}

	void remove(long long key, Table *t)
	{
		int index = key % t->size;
		int size = t->size;
		long long *p = t->keys;
		for (int i = 0; i < size; ++i)
		{
			if (p[index] == key)
			{
				p[index] = 0;

			}
			else if (p[index] == 0)
			{
				return;
			}
			++index;
			index %= size;
		}
	}

	void resize(Table *t, int new_size)
	{
		long long* tmp = (long long*)calloc(new_size, sizeof(long long));
		int original_size = t->size;
		long long* p = t->keys;
		for (int i = 0; i < original_size; ++i)
		{
			if (p[i] == 0) continue;

			long long key = p[i];
			int new_index = key % new_size;
			tmp[new_index] = key;
		}
		free(t->keys);
		t->keys = tmp;
		t->size = new_size;
	}

	
}
