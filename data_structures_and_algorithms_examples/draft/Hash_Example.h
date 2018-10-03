/*
Data structures and algorithms
Levent Albayrak 2018
*/
#pragma once
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable:4996)

/*
hash functions adapted from http://www.cse.yorku.ca/~oz/hash.html
pick any except LOSE_LOSE :)
*/
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

	//not very good, just for demonstration
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

/*
Closed hash table implementation with linear probing and modular indexing
stores void pointers
uses modular indexing, so pick a prime number for the size
*/
namespace Closed_Hash_Linear_Probe_Modular_Indexing
{
	struct Table
	{
		void **data;
		unsigned long long *keys;
		int size;//pick a prime number!!
	};

	void init(Table *t, int size)
	{
		t->size = size;
		t->data = (void**)malloc(sizeof(void*)*t->size);
		t->keys = (unsigned long long*)malloc(sizeof(unsigned long long)*t->size);

		memset(t->keys, 0, sizeof(unsigned long long)*t->size);
	}

	int set(Table *t, unsigned long long key, void *val)
	{
		int index = key % t->size;
		for (int i = 0; i < t->size; i++)
		{
			if (t->keys[index] == key) //if key exists, overwrite data
			{
				t->data[index] = val;
				return index;
			}
			if (t->keys[index] == 0) //empty spot
			{
				t->keys[index] = key;
				t->data[index] = val;
				return index;
			}
			index++;
			index %= t->size;
		}

		return -1;//return invalid index if data was not placed. this could be used as an indicator to resize.
	}

	void *get(Table *t, unsigned long long key)
	{
		int index = key % t->size;
		for (int i = 0; i < t->size; i++)
		{
			if (t->keys[index] == key)//found key
			{
				return t->data[index];
			}

			if (t->keys[index] == 0) return NULL;//if you see a zero,key was never there

			index++;
			index %= t->size;
		}

		return NULL;
	}

	void resize(Table *table, int new_size)
	{
		Table tmp;
		init(&tmp, new_size);

		for (int i = 0; i < table->size; i++)
		{
			if (table->keys[i] != 0)//rehash entries, can skip zeros because there is no data there
			{
				set(&tmp, table->keys[i], table->data[i]);
			}
		}

		free(table->data);
		free(table->keys);

		*table = tmp;
	}

}


/*
Closed hash table implementation with linear probing and multiplicative indexing
stores void pointers
size of the data array is 2^size_bits
*/
namespace Closed_Hash_Linear_Probe_Multiplicative_Indexing
{
	struct Table
	{
		void **data;
		unsigned long long *keys;
		int size_bits;
	};

	void init(Table *h, int size_bits)
	{
		h->size_bits = size_bits;
		int n_elements = 1 << size_bits;//same as "n_elements = 2^size_bits"
		h->data = (void**)malloc(sizeof(void*)*n_elements);
		h->keys = (unsigned long long*)malloc(sizeof(unsigned long long)*n_elements);

		memset(h->data, 0, sizeof(void*)*n_elements);
		memset(h->keys, 0, sizeof(unsigned long long)*n_elements);
	}

	void resize(Table *h)
	{
		Table tmp;
		init(&tmp, h->size_bits + 1);//if previous size was 2^size_bits, now it is 2^(size_bits+1) ==> 2*2^size_bits. simply doubled the size.

		int n_elements = 1 << h->size_bits;
		for (int i = 0; i < n_elements; i++)
		{
			if (h->keys[i] != 0)//rehash entries, can skip zeros because there is no data there
			{
				set(&tmp, h->keys[i], h->data[i]);
			}
		}

		free(h->keys);
		free(h->data);

		*h = tmp;
	}

	int set(Table *h, unsigned long long key, void *val)
	{
		//knuth's golden ratio multiplicative hashing for 64-bit keys
		int index = (key * 2654435761) >> (64 - h->size_bits);
		int n_elements = 1 << h->size_bits;
		for (int i = 0; i < n_elements; i++)
		{
			if (h->keys[index] == key) //if key exists, overwrite data
			{
				h->data[index] = val;
				return index;
			}

			if (h->keys[index] == 0) //empty spot
			{
				h->data[index] = val;
				h->keys[index] = key;
				return index;
			}

			index++;
			index &= n_elements - 1;
			//replaces index %=n_elements;
			/*
			explanation:
			since n_elements is 2^size_bits, n_elements is a power of 2.
			assume 'b' is a power of 2.
			then the following property holds:
			"a % b" is equal to "a & (b-1)"
			*/
		}

		return -1;//return invalid index if data was not placed. this could be used as an indicator to resize.
	}

	void *get(Table *h, unsigned long long key)
	{
		int index = (key * 2654435761) >> (64 - h->size_bits);
		int n_elements = 1 << h->size_bits;
		for (int i = 0; i < n_elements; i++)
		{
			if (h->keys[index] == key)//found original key
			{
				return h->data[index];
			}

			if (h->keys[index] == 0) return NULL;//if you see a zero,key was never there

			index++;
			index &= n_elements - 1;
		}
		return NULL;
	}
}

/*
open hash table implementation with modular indexing
stores doubles in dynamic arrays
uses modular indexing, so pick a prime number for the size
*/
namespace Open_Hash_Modular_Indexing
{
	struct Table
	{
		unsigned long long **keys;
		double **data;
		int n_rows;
		int *n_cols;
		int *n_data;
	};

	void init(Table *h, unsigned int size)
	{
		h->n_rows = size;
		h->data = (double**)malloc(sizeof(double*)*h->n_rows);
		h->keys = (unsigned long long**)malloc(sizeof(unsigned long long*)*h->n_rows);
		h->n_data = (int*)malloc(sizeof(int)*h->n_rows);
		h->n_cols = (int*)malloc(sizeof(int)*h->n_rows);

		for (int i = 0; i < h->n_rows; i++)
		{
			h->n_data[i] = 0;
			h->n_cols[i] = 10;
			h->data[i] = (double*)malloc(sizeof(double)*h->n_cols[i]);
			h->keys[i] = (unsigned long long*)malloc(sizeof(unsigned long long)*h->n_cols[i]);
		}
	}

	void set(Table *h, unsigned long long key, double val)
	{
		int which_row = key % h->n_rows;
		double *data_row = h->data[which_row];
		unsigned long long *key_row = h->keys[which_row];

		int size = h->n_cols[which_row];
		int n_data = h->n_data[which_row];

		for (int i = 0; i < n_data; i++)
		{
			if (key_row[i] == key)
			{
				data_row[i] = val;
				return;
			}
		}

		if (n_data >= size)
		{
			size += size >> 1;
			h->n_cols[which_row] = size;

			data_row = (double*)realloc(data_row, sizeof(double)*size);
			key_row = (unsigned long long*)realloc(key_row, sizeof(unsigned long long)*size);	
		}
		data_row[n_data] = val;
		key_row[n_data] = key;
		h->n_data[which_row]++;
	}

	double get(Table *h, unsigned long long key)
	{
		int which_row = key % h->n_rows;
		double *data_row = h->data[which_row];
		unsigned long long *key_row = h->keys[which_row];
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

	void resize(Table *h, int new_n_rows)
	{
		Table tmp;
		init(&tmp, new_n_rows);
		for (int i = 0; i < h->n_rows; i++)
		{
			for (int j = 0; j < h->n_data[i]; j++)
			{
				set(&tmp, h->keys[i][j], h->data[i][j]);
			}
		}

		for (int i = 0; i < h->n_rows; i++)
		{
			free(h->data[i]);
			free(h->keys[i]);
		}
		free(h->data);
		free(h->keys);

		h->n_rows = tmp.n_rows;
		h->data = tmp.data;
		h->keys = tmp.keys;
		h->n_cols = tmp.n_cols;
		h->n_data = tmp.n_data;
	}

}
