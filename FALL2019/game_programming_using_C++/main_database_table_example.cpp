#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

struct str64
{
	char str[64];
};

void init(str64* s, const char* str)
{
	strcpy(s->str, str);
}

struct Customer_Table
{
	str64* name;
	str64* address;
	int nrows;
	int maxnrows;
};

void init(Customer_Table *t, int maxnrows)
{
	t->maxnrows = maxnrows;
	t->name = (str64*)malloc(sizeof(str64)*maxnrows);
	t->address = (str64*)malloc(sizeof(str64)*maxnrows);
	t->nrows = 0;
}

int add(Customer_Table* t, const char* name, const char* address)
{

	if (t->nrows >= t->maxnrows) return -1;
	int k = t->nrows;
	strcpy(t->name[k].str, name);
	strcpy(t->address[k].str, address);
	t->nrows = k + 1;
	return k;
}

struct Supplier_Table
{
	str64* phone;
	str64* name;
	int nrows;
	int maxnrows;
};

void init(Supplier_Table *t, int maxnrows)
{
	t->maxnrows = maxnrows;
	t->name = (str64*)malloc(sizeof(str64)*maxnrows);
	t->phone = (str64*)malloc(sizeof(str64)*maxnrows);
	t->nrows = 0;
}

int add(Supplier_Table* t, const char* name, const char* phone)
{
	if (t->nrows >= t->maxnrows) return -1;
	int k = t->nrows;
	strcpy(t->name[k].str, name);
	strcpy(t->phone[k].str, phone);
	t->nrows = k + 1;
	return k;

}

int search(Supplier_Table* t, const char* name)
{
	for (int i = 0; i < t->maxnrows; ++i)
	{
		if (!strcmp(name, t->name[i].str))
		{
			return i;
		}
	}
	return -1;
}

struct Item_Table
{
	str64* name;
	int* price;
	int* supplier_id;
	int nrows;
	int maxnrows;
};

void init(Item_Table *t, int maxnrows)
{
	t->maxnrows = maxnrows;
	t->name = (str64*)malloc(sizeof(str64)*maxnrows);
	t->price = (int*)malloc(sizeof(int) * maxnrows);
	t->supplier_id = (int*)malloc(sizeof(int) * maxnrows);
	t->nrows = 0;
}

int add(Item_Table* t, const char* name, int supplier_id, int price)
{
	if (t->nrows >= t->maxnrows) return -1;
	int k = t->nrows;
	t->supplier_id[k] = supplier_id;
	strcpy(t->name[k].str, name);
	t->price[k] = price;
	t->nrows = k + 1;
	return k;
}

struct Newsletter_Table
{
	str64* name;
	int* supplier_id;
	int nrows;
	int maxnrows;
};

void init(Newsletter_Table *t, int maxnrows)
{
	t->maxnrows = maxnrows;
	t->name = (str64*)malloc(sizeof(str64)*maxnrows);
	t->supplier_id = (int*)malloc(sizeof(int) * maxnrows);
	t->nrows = 0;
}

int add(Newsletter_Table* t, const char* name, int supplier_id)
{
	if (t->nrows >= t->maxnrows) return -1;
	int k = t->nrows;
	strcpy(t->name[k].str, name);
	t->supplier_id[k] = supplier_id;
	t->nrows = k + 1;
	return k;
}

struct Subscription_Table
{
	int* customer_id;
	int* newsletter_id;
	int nrows;
	int maxnrows;
};

void init(Subscription_Table *t, int maxnrows)
{
	t->maxnrows = maxnrows;
	t->customer_id = (int*)malloc(sizeof(int) * maxnrows);
	t->newsletter_id = (int*)malloc(sizeof(int) * maxnrows);
	t->nrows = 0;
}

int add(Subscription_Table* t, int customer_id, int newsletter_id)
{
	if (t->nrows >= t->maxnrows) return -1;
	int k = t->nrows;
	t->customer_id[k] = customer_id;
	t->newsletter_id[k] = newsletter_id;
	t->nrows = k + 1;
	return k;
}

struct Sales_Table
{
	int *customer_id;
	int *item_id;
	int nrows;
	int maxnrows;
};

void init(Sales_Table *t, int maxnrows)
{
	t->maxnrows = maxnrows;
	t->customer_id = (int*)malloc(sizeof(int) * maxnrows);
	t->item_id = (int*)malloc(sizeof(int) * maxnrows);
	t->nrows = 0;
}

int add(Sales_Table* t, int customer_id, int item_id)
{
	if (t->nrows >= t->maxnrows) return -1;
	int k = t->nrows;
	t->customer_id[k] = customer_id;
	t->item_id[k] = item_id;
	t->nrows = k + 1;
	return k;
}


void print_Orders(
	Sales_Table* ts,
	Customer_Table* tc, 
	Item_Table* ti, 
	Supplier_Table* tss)
{
	str64* item_names = ti->name;
	str64* customer_names = tc->name;
	int* prices = ti->price;
	str64* supplier_names = tss->name;

	printf("print orders\n");
	for (int i = 0; i < ts->nrows; ++i)
	{
		int item_id = ts->item_id[i];
		int customer_id = ts->customer_id[i];
		int supplier_id = ti->supplier_id[item_id];

		printf("%s %s %s %d\n",
			customer_names[customer_id].str,
			supplier_names[supplier_id].str,
			item_names[item_id].str,
			prices[item_id]
		);
	}
}

void print_Subscriptions(Customer_Table *c, Newsletter_Table *n, Subscription_Table *s)
{
	str64* customer_names = c->name;
	str64* newsletter_names = n->name;

	printf("newsletter subs\n");
	for (int i = 0; i < s->nrows; ++i)
	{
		int customer_id = s->customer_id[i];
		int newsletter_id = s->newsletter_id[i];

		printf("%s %s\n", customer_names[customer_id].str, newsletter_names[newsletter_id].str);
	}
}



int main()
{
	Customer_Table tcustomer;
	Supplier_Table tsupplier;
	Newsletter_Table tnewsletter;
	Subscription_Table tsubscription;
	Item_Table titem;
	Sales_Table tsales;

	init(&tcustomer, 1000);
	init(&tsupplier, 1000);
	init(&tnewsletter, 1000);
	init(&tsubscription, 1000);
	init(&titem, 1000);
	init(&tsales, 1000);

	int MarkID = add(&tcustomer, "Mark", "123 Sesame Street");
	add(&tcustomer, "Ale", "123 Fresh Street");
	add(&tcustomer, "Hollie", "321 Sesame Street");
	int KarlID = add(&tcustomer, "Karl", "321 Sesame Street");

	add(&tsupplier, "Microsoft", "123-456-789");
	int sonysupplierID = add(&tsupplier, "Sony", "800-XBOX-789");

	add(&tnewsletter, "XBOX NEWS TODAY", search(&tsupplier, "Microsoft"));
	add(&tnewsletter, "PS4 MONTHLY", sonysupplierID);
	add(&tnewsletter, "PSVR DEALS", sonysupplierID);

	add(&tsubscription, MarkID, 0);
	add(&tsubscription, MarkID, 1);
	add(&tsubscription, MarkID, 2);
	add(&tsubscription, KarlID, 1);

	add(&titem, "PS4", 1, 400);
	add(&titem, "Xbox", 0, 300);
	add(&titem, "PSVR", 1, 350);

	for(int i = 0; i < tcustomer.nrows; ++i)
	{
		add(&tsales, i, rand() % titem.nrows);
	}

	//turn into function
	//str64* item_names = titem.name;
	//str64* customer_names = tcustomer.name;
	//int* prices = titem.price;
	//str64* supplier_names = tsupplier.name;

	//printf("print orders\n");
	//for (int i = 0; i < tsales.nrows; ++i)
	//{
	//	int item_id = tsales.item_id[i];
	//	int customer_id = tsales.customer_id[i];
	//	int supplier_id = titem.supplier_id[item_id];

	//	printf("%s %s %s %d\n",
	//		customer_names[customer_id].str,
	//		supplier_names[supplier_id].str,
	//		item_names[item_id].str,
	//		prices[item_id]
	//	);
	//}


	print_Orders(&tsales, &tcustomer, &titem, &tsupplier);
	print_Subscriptions(&tcustomer, &tnewsletter, &tsubscription);

	getchar();
	return 0;
}