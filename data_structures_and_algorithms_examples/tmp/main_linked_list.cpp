#include <stdio.h>
#include <stdlib.h>


struct Node
{
	int data;
	Node *next;
};

void add(Node *&head, int data)
{
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->data = data;
	new_node->next = head;
	head = new_node;
}

Node* remove(Node *&head)
{
	Node *ret = head;
	head = head->next;
	return ret;
}

void print(Node *head)
{
	Node *current = head;
	while (current != NULL)
	{
		printf("%d\n", current->data);
		current = current->next;
	}
}

int main()
{
	Node *a = (Node*)malloc(sizeof(Node));;
	a->data = 1;
	a->next = NULL;

	Node *head = a;

	add(head, 3);
	add(head, 2);
	remove(head);
	print(head);
	

	getchar();
	return 0;
}


