#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

struct Node
{
	char data;
	Node* left;
	Node* right;
};

void insert(Node* parent, Node* child)
{
	if (parent == NULL) return;
	printf("child:%c->parent:%c\n", child->data, parent->data);
	if (parent->left == NULL)
	{
		parent->left = child;
		return;
	}
	if (parent->right == NULL)
	{
		parent->right = child;
		return;
	}
	if (parent->left->left == NULL || parent->left->right == NULL)
	{
		insert(parent->left, child);
		return;
	}
	if (parent->right->left == NULL || parent->right->right == NULL)
	{
		insert(parent->right, child);
		return;
	}
	
}

void mark_preorder(Node* root)
{
	if (root == NULL) return;
	printf("%c\n", root->data);
	mark_preorder(root->left);
	mark_preorder(root->right);
}

void mark_inorder(Node* root)
{
	if (root == NULL) return;
	mark_inorder(root->left);
	printf("%c\n", root->data);
	mark_inorder(root->right);
}

void mark_postorder(Node* root)
{
	if (root == NULL) return;
	mark_postorder(root->left);
	mark_postorder(root->right);
	printf("%c\n", root->data);
}

int main()
{
	Node nodes[10] = { 0 };
	nodes[0].data = 'A';
	nodes[1].data = 'B';
	nodes[2].data = 'C';
	nodes[3].data = 'D';
	nodes[4].data = 'E';
	nodes[5].data = 'F';
	nodes[6].data = 'G';

	Node* root = &nodes[0];

	insert(root, &nodes[1]);
	insert(root, &nodes[2]);
	insert(root, &nodes[3]);
	insert(root, &nodes[4]);
	insert(root, &nodes[5]);
	insert(root, &nodes[6]);

	printf("PREORDER\n");
	mark_preorder(root);

	printf("INORDER\n");
	mark_inorder(root);

	printf("POSTORDER\n");
	mark_postorder(root);

	getchar();
	return 0;
}