#include <iostream>
using namespace std;

namespace Closed_Hash
{
	struct Table
	{
		void **data;
		unsigned long long *keys;
		int size_bits;
	};

	unsigned long long encrypt_djb2(char *data, int n_bytes);

	void init(Table *h, int size_bits);

	void resize(Table *h);

	void set(Table *h, unsigned long long key, void *val);

	void *get(Table *h, unsigned long long key);

}

namespace Closed_Hash
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

	void init(Table *h, int size_bits)
	{
		h->size_bits = size_bits;
		int n_elements = 1 << size_bits;
		h->data = (void**)malloc(sizeof(void*)*n_elements);
		h->keys = (unsigned long long*)malloc(sizeof(unsigned long long)*n_elements);

		memset(h->data, 0, sizeof(void*)*n_elements);
		memset(h->keys, 0, sizeof(unsigned long long)*n_elements);
	}

	void resize(Table *h)
	{
		Table tmp;
		init(&tmp, h->size_bits + 1);

		int n_elements = 1 << h->size_bits;
		for (int i = 0; i < n_elements; i++)
		{
			if (h->keys[i] != 0)
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
		int index = (key * 2654435761) >> (64-h->size_bits);
		int n_elements = 1 << h->size_bits;
		for (int i = 0; i < n_elements; i++)
		{
			if (h->keys[index] == 0)
			{
				h->data[index] = val;
				h->keys[index] = key;
				return index;
			}
			index++;
			index &= n_elements - 1;
		}

		return 0;
	}

	void *get(Table *h, unsigned long long key)
	{
		int index = (key * 2654435761) >> (64 - h->size_bits);
		int n_elements = 1 << h->size_bits;
		for (int i = 0; i < n_elements; i++)
		{
			if (h->keys[index] == 0) return NULL;

			if (h->keys[index] == key)
			{
				return h->data[index];
			}
			index++;
			index &= n_elements - 1;
		}
		return NULL;
	}

	
}

int main()
{
	Closed_Hash::Table h;
	Closed_Hash::init(&h, 2);

	unsigned long long key;
	void *val;

	val = "Best Karthus NA (rank 1)";
	key = Closed_Hash::encrypt_djb2("Levent", strlen("Levent"));
	Closed_Hash::set(&h, key, val);

	val = "Nicest person ever!";
	key = Closed_Hash::encrypt_djb2("Alexander", strlen("Alexander"));
	Closed_Hash::set(&h, key, val);

	val = "Nicest person ever too!";
	key = Closed_Hash::encrypt_djb2("Valjeaun", strlen("Valjeaun"));
	Closed_Hash::set(&h, key, val);

	val = "MAKE STUFF UP";
	key = Closed_Hash::encrypt_djb2("Leo", strlen("Leo"));
	Closed_Hash::set(&h, key, val);

	val = "VERY GOOD AT RPG GAMES";
	key = Closed_Hash::encrypt_djb2("Marco", strlen("Marco"));
	Closed_Hash::set(&h, key, val);

	val = "ALWAYS LATE";
	key = Closed_Hash::encrypt_djb2("Daniel", strlen("Daniel"));
	Closed_Hash::set(&h, key, val);

	val = "The King";
	key = Closed_Hash::encrypt_djb2("Terence", strlen("Terence"));
	Closed_Hash::set(&h, key, val);

	val = "Lightning truck";
	key = Closed_Hash::encrypt_djb2("Ryan", strlen("Ryan"));
	Closed_Hash::set(&h, key, val);

	val = "has a book";
	key = Closed_Hash::encrypt_djb2("James", strlen("James"));
	Closed_Hash::set(&h, key, val);

	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("James", strlen("James"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Ryan", strlen("Ryan"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Terence", strlen("Terence"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Marco", strlen("Marco"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Daniel", strlen("Daniel"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Alexander", strlen("Alexander"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Leo", strlen("Leo"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Valjeaun", strlen("Valjeaun"))) << endl;
	cout << (char*)Closed_Hash::get(&h, Closed_Hash::encrypt_djb2("Levent", strlen("Levent"))) << endl;



	getchar();
	return 0;
}