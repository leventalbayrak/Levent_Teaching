#include <iostream>
using namespace std;

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
	struct Data
	{
		int key;
		double data;
	};
	struct Key_Value_Pair
	{
		Data **table;
		int n_rows;
		int *n_cols;
		int *n_data;
	};

	void init(Key_Value_Pair *h, unsigned int size)
	{
		h->n_rows = size;
		h->table = (Data**)malloc(sizeof(Data*)*h->n_rows);
		h->n_data = (int*)malloc(sizeof(int)*h->n_rows);
		h->n_cols = (int*)malloc(sizeof(int)*h->n_rows);

		for (int i = 0; i < h->n_rows; i++)
		{
			h->n_data[i] = 0;
			h->n_cols[i] = 10;
			h->table[i] = (Data*)malloc(sizeof(Data)*h->n_cols[i]);
		}
	}

	void set(Key_Value_Pair *h, int key, int val)
	{
		int which_row = key % h->n_rows;
		Data *row = h->table[which_row];
		int n_data = h->n_data[which_row];
		row[n_data].data = val;
		row[n_data].key = key;
		h->n_data[which_row]++;
	}

	double get(Key_Value_Pair *h, int key)
	{
		int which_row = key % h->n_rows;
		Data *row = h->table[which_row];
		int n_data = h->n_data[which_row];
		for (int i = 0; i < n_data; i++)
		{
			if (row[i].key == key)
			{
				return row[i].data;
			}
		}
		return 0.0;
	}
}

int main()
{

	Key_Value_Pair::Key_Value_Pair h;
	Key_Value_Pair::init(&h, 10007);

	Key_Value_Pair::set(&h, 10007, 120);
	Key_Value_Pair::set(&h, 10007*2, 130);

	cout << Key_Value_Pair::get(&h, 10007) << endl;
	cout << Key_Value_Pair::get(&h, 10007*2) << endl;
	//cout << Key_Value_Pair::get(&h, Encryption::encrypt_djb2("Leo", 3)) << endl;

	getchar();
	return 0;
}

