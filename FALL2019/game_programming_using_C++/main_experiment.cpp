
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

unsigned long long Mark2019_SPACEFLIGHT2oNeOH(const char* name)
{
//	printf("using cryptographic function Mark2019_SPACEFLIGHT2oNeOH(TM)\n");
	unsigned long long sum = 0;
	int len = strlen(name);
	for (int i = 0; i < len; ++i) sum += name[i];
	return sum;
}

unsigned long long Hollie_SNOWPIERCER_v1201274_64bit(const char* name)
{
//	printf("using cryptographic function Hollie_SNOWPIERCER_v1201274_64bit(TM)\n");
	unsigned long long sum = 0;
	int len = strlen(name);
	for (int i = 0; i < len; ++i) sum += name[i] * len / (i+1);

	return sum;
}

unsigned long long djb2(const char *str)
{
	unsigned long long hash = 5381;
	int len = strlen(str);
	for (int i = 0; i < len; ++i)
	{
		//hash = (hash << 5 + hash) + str[i];//FASTER
		//hash = (hash * 32 + hash) + str[i];//EXPLANATION
		hash = hash * 33 + str[i];
	}
	return hash;
}

unsigned long long sdbm(const char *str)
{
	unsigned long long hash = 0;
	int len = strlen(str);
	for (int i = 0; i < len; ++i)
	{
		hash = str[i] + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

int main()
{
	srand(time(0));
	
	int array_size = 1000039;
	
	unsigned long long *hash_table[3];
	hash_table[0] = (unsigned long long*)malloc(sizeof(unsigned long long)*array_size);
	hash_table[1] = (unsigned long long*)malloc(sizeof(unsigned long long)*array_size);
	hash_table[2] = (unsigned long long*)malloc(sizeof(unsigned long long)*array_size);
	//memset(hash_table, 0, sizeof(long long)*array_size);
	for (int i = 0; i < array_size; i++)
	{
		hash_table[0][i] = -1;
		hash_table[1][i] = -1;
		hash_table[2][i] = -1;
	}

	int same_key[3] = { 0 };
	int collision_counter[3] = { 0 };

	for (int ale = 0; ale < 1000; ale++)
	{
		//get input
		char name[100];
		int len = 50 + rand() % 50;
		for (int i = 0; i < len; i++)
		{
			name[i] = 'a' + rand() % ('z'-'a');
		}
		name[len] = 0;

		//ADD
		//step 1

		//generate key
		{
			unsigned long long key = Hollie_SNOWPIERCER_v1201274_64bit(name);
			//get index
			int index = key % array_size;

			//search for the next available spot
			for (int __i = 0; __i < array_size; ++__i)
			{
				if (hash_table[0][index] == -1)
				{
					hash_table[0][index] = key;
					break;
				}
				else if (hash_table[0][index] == key)
				{
					++collision_counter[0];
					++same_key[0];
					break;
				}
				else
				{
					++collision_counter[0];
					++index;
					index %= array_size;
				}
			}
		}
		{
			unsigned long long key = djb2(name);
			//get index
			int index = key % array_size;

			//search for the next available spot
			for (int __i = 0; __i < array_size; ++__i)
			{
				if (hash_table[1][index] == -1)
				{
					hash_table[1][index] = key;
					break;
				}
				else if (hash_table[1][index] == key)
				{
					++collision_counter[1];
					++same_key[1];
					break;
				}
				else
				{
					++collision_counter[1];
					++index;
					index %= array_size;
				}
			}
		}
		{
			unsigned long long key = sdbm(name);
			//get index
			int index = key % array_size;

			//search for the next available spot
			for (int __i = 0; __i < array_size; ++__i)
			{
				if (hash_table[2][index] == -1)
				{
					hash_table[2][index] = key;
					break;
				}
				else if (hash_table[2][index] == key)
				{
					++collision_counter[2];
					++same_key[2];
					break;
				}
				else
				{
					++collision_counter[2];
					++index;
					index %= array_size;
				}
			}
		}
	}

	printf("MARK collision_counter: %d same_key: %d\n", collision_counter[0], same_key[0]);
	printf("djb2 collision_counter: %d same_key: %d\n", collision_counter[1], same_key[1]);
	printf("sdbm collision_counter: %d same_key: %d\n", collision_counter[2], same_key[2]);
	
	getchar();
	return 0;
}


