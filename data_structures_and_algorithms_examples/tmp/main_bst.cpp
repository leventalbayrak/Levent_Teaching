#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct BST_Node
{
	int data;
	BST_Node *left;
	BST_Node *right;
};

BST_Node *create(int data)
{
	BST_Node *n = (BST_Node*)malloc(sizeof(BST_Node));
	n->left = NULL;
	n->right = NULL;
	n->data = data;
	return n;
}

void add(BST_Node *root, int data)
{
	if (data < root->data)
	{
		if (root->left == NULL)
		{
			root->left = create(data);
			return;
		}
		add(root->left, data);
	}
	else
	{
		if (root->right == NULL)
		{
			root->right = create(data);
			return;
		}
		add(root->right, data);
	}
}

int search(BST_Node *root, int data)
{
	if (root == NULL) return 0;

	if (root->data == data) return 1;

	if (data < root->data) return search(root->left, data);

	return search(root->right, data);
}

void print_Preorder(BST_Node *root)
{
	if (root == NULL) return;
	printf("%d,", root->data);
	print_Preorder(root->left);
	print_Preorder(root->right);
}

void print_Inorder_Descending(BST_Node *root)
{
	if (root == NULL) return;
	print_Inorder_Descending(root->left);
	printf("%d,", root->data);
	print_Inorder_Descending(root->right);
}

void print_Inorder_Ascending(BST_Node *root)
{
	if (root == NULL) return;
	print_Inorder_Ascending(root->right);
	printf("%d,", root->data);
	print_Inorder_Ascending(root->left);
}

void print_Postorder(BST_Node *root)
{
	if (root == NULL) return;
	print_Postorder(root->left);
	print_Postorder(root->right);
	printf("%d,", root->data);
}

int main()
{
	srand(8);

	BST_Node root = {};

	const int n_size = 5;
	static int numbers[n_size];

	for (int i = 0; i < n_size; i++) numbers[i] = rand() % 100;
	
	root.data = numbers[0];
	for (int i = 1; i < n_size; i++)
	{
		add(&root, numbers[i]);
	}

	for (int i = 0; i < n_size; i++)
	{
		if (search(&root, numbers[i]) != 1)
		{
			printf("could not find anything you are fired!\n");
		}
	}

	for (int i = 0; i < n_size; i++)
	{
		int n = 100 + rand() % 100;
		if (search(&root, n) != 0)
		{
			printf("found garbage you are fired!\n");
		}
	}
	
	printf("you survive another day\n");

	for (int i = 0; i < n_size; i++) printf("%d,", numbers[i]);
	printf("\n");
	
	print_Preorder(&root);
	printf("\n");
	print_Inorder_Ascending(&root);
	printf("\n");
	print_Postorder(&root);
	printf("\n");

	getchar();
	return 0;
}