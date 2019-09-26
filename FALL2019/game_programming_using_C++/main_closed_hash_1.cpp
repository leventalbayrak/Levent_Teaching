
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

long long Mark2019_SPACEFLIGHT2oNeOH(const char* name)
{
	printf("using cryptographic function Mark2019_SPACEFLIGHT2oNeOH(TM)\n");
	long long sum = 0;
	int len = strlen(name);
	for (int i = 0; i < len; ++i) sum += name[i];
	return sum;
}

long long Hollie_SNOWPIERCER_v1201274_64bit(const char* name)
{
	printf("using cryptographic function Hollie_SNOWPIERCER_v1201274_64bit(TM)\n");
	long long sum = 0;
	int len = strlen(name);
	for (int i = 0; i < len; ++i) sum += name[i] * len / (i+1);

	return sum;
}

int main()
{
	int array_size = 7;
	long long *hash_table = (long long*)malloc(sizeof(long long)*array_size);
	memset(hash_table, 0, sizeof(long long)*array_size);

	for (;;)
	{
		//get input
		char name[10];
		printf("enter a name:");
		scanf("%s", name);

		//ADD
		//step 1
		//generate key
		long long key = Mark2019_SPACEFLIGHT2oNeOH(name);
		//get index
		int index = key % array_size;

		printf("%s key: %lld index: %d\n", name, key, index);
		//search for the next available spot
		for (int __i = 0; __i < array_size; ++__i)
		{
			if (hash_table[index] == 0)
			{
				hash_table[index] = key;

				printf("found empty spot at %d\n", index);
				break;
			}
			else if (hash_table[index] == key)
			{
				printf("%lld already exists!\n", key);
				break;
			}
			else
			{
				printf("index %d full, try again %d\n",index, __i);
				++index;
				index %= array_size;
			}
		}
		

	}

	return 0;
}


