#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Binary_Search_Tree_Example.h"

void print_Preorder(BST::Node *root)
{
	if (root == NULL) return;
	printf("%d,", root->data);
	print_Preorder(root->left);
	print_Preorder(root->right);
}

void print_Inorder_Descending(BST::Node *root)
{
	if (root == NULL) return;
	print_Inorder_Descending(root->left);
	printf("%d,", root->data);
	print_Inorder_Descending(root->right);
}

void print_Inorder_Ascending(BST::Node *root)
{
	if (root == NULL) return;
	print_Inorder_Ascending(root->right);
	printf("%d,", root->data);
	print_Inorder_Ascending(root->left);
}

void print_Postorder(BST::Node *root)
{
	if (root == NULL) return;
	print_Postorder(root->left);
	print_Postorder(root->right);
	printf("%d,", root->data);
}

int main()
{
	srand(8);

	BST::Node root = {};

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
	
	printf("original list: ");
	for (int i = 0; i < n_size; i++) printf("%d,", numbers[i]);
	printf("\n");
	
	printf("pre-order: ");
	print_Preorder(&root);
	printf("\nin-order ascending: ");
	print_Inorder_Ascending(&root);
	printf("\npost-order: ");
	print_Postorder(&root);

	getchar();
	return 0;
}