#pragma once
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable:4996)

namespace Encryption
{
	unsigned long long encrypt_djb2(char *data, int n_bytes)
	{
		unsigned long long h = 5381;

		for (int i = 0; i < n_bytes; i++)
		{
			h = 33 * h + data[i];
		}
		return h;
	}

	unsigned long long encrypt_sdbm(char *data, int n_bytes)
	{
		unsigned long long h = 0;

		for (int i = 0; i < n_bytes; i++)
		{
			h = 65599 * h + data[i];
		}
		return h;
	}

	unsigned long long encrypt_LOSE_LOSE(char *data, int n_bytes)
	{
		unsigned long long h = 0;

		for (int i = 0; i < n_bytes; i++)
		{
			h += data[i];
		}
		return h;
	}
}

namespace Key_Value_Pair
{
	struct Key_Value_Pair
	{
		long long int **keys;
		double **data;
		int n_rows;
		int *n_cols;
		int *n_data;
	};

	void init(Key_Value_Pair *h, unsigned int size)
	{
		h->n_rows = size;
		h->data = (double**)malloc(sizeof(double*)*h->n_rows);
		h->keys = (long long int**)malloc(sizeof(long long int*)*h->n_rows);
		h->n_data = (int*)malloc(sizeof(int)*h->n_rows);
		h->n_cols = (int*)malloc(sizeof(int)*h->n_rows);

		for (int i = 0; i < h->n_rows; i++)
		{
			h->n_data[i] = 0;
			h->n_cols[i] = 10;
			h->data[i] = (double*)malloc(sizeof(double)*h->n_cols[i]);
			h->keys[i] = (long long int*)malloc(sizeof(long long int)*h->n_cols[i]);
		}
	}

	void set(Key_Value_Pair *h, long long int key, int val)
	{
		int which_row = key % h->n_rows;
		double *data_row = h->data[which_row];
		long long int *key_row = h->keys[which_row];
		int n_data = h->n_data[which_row];
		data_row[n_data] = val;
		key_row[n_data] = key;
		h->n_data[which_row]++;
	}

	double get(Key_Value_Pair *h, long long int key)
	{
		int which_row = key % h->n_rows;
		double *data_row = h->data[which_row];
		long long int *key_row = h->keys[which_row];
		int n_data = h->n_data[which_row];
		for (int i = 0; i < n_data; i++)
		{
			if (key_row[i] == key)
			{
				return data_row[i];
			}
		}
		return 0.0;
	}
}

