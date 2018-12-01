#pragma once

namespace Trie
{

	struct Node
	{
		int count;
		Node **branches;
	};

	void init(Node *n)
	{
		n->count = 0;
		n->branches = (Node**)malloc(sizeof(Node*) * 256);
		memset(n->branches, 0, sizeof(Node*) * 256);
	}

	void add_Word(Node *root, const unsigned char *word, int length)
	{
		Node *current = root;

		for (int i = 0; i < length; i++)
		{
			if (current->branches[word[i]] == NULL)
			{
				current->branches[word[i]] = (Node*)malloc(sizeof(Node));
				init(current->branches[word[i]]);
			}
			current = current->branches[word[i]];
			current->count++;
		}
	}

	int search(Node *root, const unsigned char* query, int length)
	{
		Node *current = root;

		for (int i = 0; i < length; i++)
		{
			if (current->branches[query[i]] == NULL)
			{
				return 0;
			}
			current = current->branches[query[i]];
		}
		return current->count;
	}

	void add_Suffix(Node *root, const unsigned char *word, int length)
	{
		for (int i = 0; i < length; i++)
		{
			add_Word(root, &word[i], length - i);
		}
	}

	void add_Disassembly(Node *root, const unsigned char *word, int length, int subsequence_length)
	{
		for (int i = 0; i <= length - subsequence_length; i++)
		{
			add_Word(root, &word[i], subsequence_length);
		}
	}

	char pick_Random(Node *root, const unsigned char *subsequence, int subsequence_length)
	{
		Node *current = root;

		for (int i = 0; i < subsequence_length; i++)
		{
			if (current->branches[subsequence[i]] == NULL)
			{
				return 0;
			}
			current = current->branches[subsequence[i]];
		}

		int p = 1 + rand() % current->count;

		int sum = 0;
		for (int i = 0; i < 256; i++)
		{
			if (current->branches[i] != NULL)
			{
				sum += current->branches[i]->count;
				if (sum >= p) return i;
			}
		}
	}
}