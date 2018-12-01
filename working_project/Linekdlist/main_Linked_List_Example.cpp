
#include <stdio.h>
#include <stdlib.h>

#include "Linked_List_Example.h"

int main()
{
	Linked_List::Linked_List list;
	Linked_List::init(&list);

	Linked_List::add(&list, 1);
	Linked_List::add(&list, 3);
	Linked_List::add(&list, 2);
	Linked_List::add(&list, 4);

	printf("list before remove\n");
	Linked_List::print(&list);

	Linked_List::remove(&list);

	printf("list after remove\n");
	Linked_List::print(&list);
	
	getchar();
	return 0;
}


