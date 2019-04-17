
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rect
{
	float x, y, w, h;
};

struct Rect_Table
{
	unsigned char *active;
	Rect *data;
	int array_size;
};

void init(Rect_Table *table, int array_size)
{
	table->array_size = array_size;
	table->active = (unsigned char*)malloc(sizeof(unsigned char)*array_size);
	table->data = (Rect*)malloc(sizeof(Rect)*array_size);
	memset(table->active, 0, sizeof(unsigned char)*array_size);
}

int create(Rect_Table *table)
{
	for (int i = 0; i < table->array_size; i++)
	{
		if (table->active[i] == 0)
		{
			table->active[i] = 1;
			return i;
		}
	}

	return -1;
}

void destroy(int which, Rect_Table *table)
{
	table->active[which] = 0;
}

void print(Rect_Table *table)
{
	for (int i = 0; i < table->array_size; i++)
	{
		if (table->active[i] == 1)
		{
			printf("%f %f %f %f\n", table->data[i].x, table->data[i].y, table->data[i].w, table->data[i].h);
		}
	}
}

int main()
{
	
	Rect_Table table;
	init(&table, 100);

	int r = create(&table);
	destroy(r, &table);

	return 0;
}