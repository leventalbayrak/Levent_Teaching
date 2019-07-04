#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

struct Item
{
	int id;
	float price;
};


int main()
{
	Item a;
	//dot offset operation
	a.id = 5;
	a.price = 8;

	Item *b = &a;
	b->id = 3;
	b->price = 9;

	//arrow alternative
	(*b).id = 7;
	(*b).price = 17;

	printf("&a: %u &a.id: %u &a.price: %u\n", &a, &a.id, &a.price);

	Item *items = (Item *)malloc(sizeof(Item) * 10);

	Item itemz[10];

	items[5].id = 9;
	items[5].price = 8;

	//sets price of first item (index 0) to zero
	items->price = 0;
	(*items).price = 0;

	Item **itemzz = (Item **)malloc(sizeof(Item*) * 10);

	itemzz[9] = &a;
	itemzz[9]->price = 5;
	(*itemzz[9]).price = 5;

	getchar();
	return 0;
}