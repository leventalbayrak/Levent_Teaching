#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

struct Node
{
	int data;
	Node* left;
	Node* right;
};

void add(Node* root, Node* n)
{

	if (n->data < root->data)
	{
		if (root->left == NULL)
		{
			root->left = n;
		}
		else
		{
			add(root->left, n);
		}
		
	}
	else
	{
		if (root->right == NULL)
		{
			root->right = n;
		}
		else
		{
			add(root->right, n);
		}
	}
}

int search(Node* root, int value)
{
	if (root == NULL) return 0;

	if (root->data == value) return 1;
	if (value < root->data)
	{
		return search(root->left, value);
	}
	else
	{
		return search(root->right, value);
	}
}

void print(Node* root)
{
	if (root == NULL) return;

	print(root->left);
	printf("%d\n", root->data);
	print(root->right);
}

int main()
{
	Node root = { 0 };
	root.data = rand()%100;
	printf("root %d\n", root.data);
	Node nodes[5] = { 0 };

	for (int i = 0; i < 5; ++i)
	{
		nodes[i].data = rand() % 100;
		printf("%d\n", nodes[i].data);
		add(&root, &nodes[i]);
	}
	
	printf("-----\n");
	print(&root);

	printf("-----\n");
	printf("%d\n", search(&root, 34));

	getchar();
	return 0;
}