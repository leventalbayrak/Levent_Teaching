#include <stdio.h>
#include <stdlib.h>

struct Node
{
	int data;
	Node* next;
};

struct List
{
	Node* head;
};

void push(List* p, Node* n)
{
	n->next = p->head;
	p->head = n;
}

Node* pop(List* p)
{
	Node* n = p->head;
	p->head = p->head->next;
	return n;
}

int main()
{

	List list = { 0 };

	{
		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data = 7;
		push(&list, tmp);
	}
	{
		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data = 3;
		push(&list, tmp);
	}
	{
		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data = 8;
		push(&list, tmp);
	}

	{
		Node* tmp = pop(&list);
		printf("%d\n", tmp->data);
	}
	{
		Node* tmp = pop(&list);
		printf("%d\n", tmp->data);
	}
	{
		Node* tmp = pop(&list);
		printf("%d\n", tmp->data);
	}

	getchar();
	return 0;
}