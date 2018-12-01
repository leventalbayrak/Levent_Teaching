#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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


//------------DONE-------------
//
//void words_Only_Experiment()
//{
//	Node root;
//	init(&root);
//	add_Word(&root, (unsigned char*)"apple", strlen("apple"));
//	printf("---\n");
//	add_Word(&root, (unsigned char*)"apricot", strlen("apricot"));
//	printf("---\n");
//	add_Word(&root, (unsigned char*)"apron", strlen("apron"));
//	printf("---\n");
//	add_Word(&root, (unsigned char*)"application", strlen("application"));
//	printf("---\n");
//
//	for (;;)
//	{
//		static unsigned char str[512];
//		printf("enter a prefix >");
//		scanf("%s", str);
//		int r = search(&root, str, strlen(str));
//		printf("%d copies found\n", r);
//	}
//
//}
//
//void suffix_Experiment()
//{
//	Node root;
//	init(&root);
//	add_Suffix(&root, "apple", strlen("apple"));
//
//	add_Suffix(&root, "apricot", strlen("apricot"));
//
//	add_Suffix(&root, "apron", strlen("apron"));
//
//	add_Suffix(&root, "application", strlen("application"));
//
//
//	for (;;)
//	{
//		static unsigned char str[512];
//		printf("enter a prefix >");
//		scanf("%s", str);
//		int r = search(&root, str, strlen(str));
//		printf("%d copies found\n", r);
//	}
//
//}
//
//void disassembly_Experiment()
//{
//	int subsequence_length = 3;
//	Node root;
//	init(&root);
//	add_Disassembly(&root, "apple", strlen("apple"), subsequence_length);
//
//	add_Disassembly(&root, "apricot", strlen("apricot"), subsequence_length);
//
//	add_Disassembly(&root, "apron", strlen("apron"), subsequence_length);
//
//	add_Disassembly(&root, "application", strlen("application"), subsequence_length);
//
//
//	for (;;)
//	{
//		static unsigned char str[512];
//		printf("enter a prefix >");
//		scanf("%s", str);
//		int r = search(&root, str, strlen(str));
//		printf("%d copies found\n", r);
//	}
//
//}

void generate_Words()
{
	int subsequence_length = 8;
	Node root;
	init(&root);

	FILE *f_out = fopen("out.txt", "w+");

	FILE *f = fopen("dracula.txt", "rb");
	fseek(f, 0, SEEK_END);
	int filesize = ftell(f);
	fseek(f, 0, SEEK_SET);
	unsigned char *data = (unsigned char*)malloc(filesize+1);
	fread(data, 1, filesize, f);
	fclose(f);

	data[filesize] = 0;
	add_Disassembly(&root, data, filesize, subsequence_length);

	int buffer_size = 10000;
	unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char)*buffer_size);

	for (int i = 0; i < subsequence_length; i++) buffer[i] = data[i];

	int position_in_buffer = 0;
	for (;;)
	{
		if (position_in_buffer >= buffer_size - subsequence_length) break;

		char c = pick_Random(&root, &buffer[position_in_buffer], subsequence_length-1);
		buffer[position_in_buffer + subsequence_length - 1] = c;
		position_in_buffer++;

	
	}
	
	buffer[buffer_size] = 0;

	fprintf(f_out,"%s\n", buffer);

}

int main()
{
	srand(time(0));
	generate_Words();

	getchar();
	return 0;
}