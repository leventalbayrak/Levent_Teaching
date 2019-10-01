#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

#include "Table_core.h"
#include "Table_ext.h"

long long generate_Key(const unsigned char* data, int n)
{
	long long int x = 5381;
	for (int i = 0; i < n; ++i) x = (x << 5) + x + data[i];
	return x;
}

int main()
{

	Table::Table table;
	Table::init(&table, 1000003);

	long long key = generate_Key((const unsigned char*)"Mark", strlen("Mark"));
	Table::insert(key, &table);

	key = generate_Key((const unsigned char*)"Karl", strlen("Karl"));
	Table::insert(key, &table);

	if (Table::search(generate_Key((const unsigned char*)"Karl", strlen("Karl")), &table))
	{
		printf("Karl found!\n");
	}

	if (Table::search(generate_Key((const unsigned char*)"Mark", strlen("Mark")), &table))
	{
		printf("Mark found!\n");
	}

	if (Table::search(generate_Key((const unsigned char*)"Ale", strlen("Ale")), &table))
	{
		printf("Ale found!\n");
	}

	Table::resize(&table, 10007);

	if (Table::search(generate_Key((const unsigned char*)"Karl", strlen("Karl")), &table))
	{
		printf("Karl found!\n");
	}

	if (Table::search(generate_Key((const unsigned char*)"Mark", strlen("Mark")), &table))
	{
		printf("Mark found!\n");
	}

	if (Table::search(generate_Key((const unsigned char*)"Ale", strlen("Ale")), &table))
	{
		printf("Ale found!\n");
	}

	getchar();
	return 0;
}