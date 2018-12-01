#pragma once

#include <stdio.h>
#include <stdlib.h>

namespace BST
{

	struct Node
	{
		int data;
		Node *left;
		Node *right;
	};

	Node *create(int data)
	{
		Node *n = (Node*)malloc(sizeof(Node));
		n->left = NULL;
		n->right = NULL;
		n->data = data;
		return n;
	}

	void add(Node *root, int data)
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

	int search(Node *root, int data)
	{
		if (root == NULL) return 0;

		if (root->data == data) return 1;

		if (data < root->data) return search(root->left, data);

		return search(root->right, data);
	}


}