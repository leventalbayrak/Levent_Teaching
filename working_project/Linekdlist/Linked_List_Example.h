#pragma once
#include <stdio.h>
#include <stdlib.h>

namespace Linked_List
{
	struct Node
	{
		int data;
		Node *next;
	};

	struct Linked_List
	{
		Node *head;
	};

	void init(Linked_List *l)
	{
		l->head = NULL;
	}

	void add(Linked_List *l, int data)
	{
		if (l->head == NULL)
		{
			l->head = (Node*)malloc(sizeof(Node));
			l->head->next = NULL;
			l->head->data = data;
			return;
		}

		Node *new_head = (Node*)malloc(sizeof(Node));
		new_head->data = data;
		new_head->next = l->head;
		
		l->head = new_head;
	}

	Node* remove(Linked_List *l)
	{
		Node *ret = l->head;
		l->head = l->head->next;
		return ret;
	}

	void print(Linked_List *l)
	{
		Node *current = l->head;
		while (current != NULL)
		{
			printf("%d\n", current->data);
			current = current->next;
		}
	}
}