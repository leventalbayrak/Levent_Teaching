#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Trie_Example.h"

void words_Only_Experiment()
{
	Trie::Node root;
	Trie::init(&root);

	Trie::add_Word(&root, (unsigned char*)"apple", strlen("apple"));
	Trie::add_Word(&root, (unsigned char*)"apricot", strlen("apricot"));
	Trie::add_Word(&root, (unsigned char*)"apron", strlen("apron"));
	Trie::add_Word(&root, (unsigned char*)"application", strlen("application"));

	for (;;)
	{
		static unsigned char str[512];
		printf("enter a prefix >");
		scanf("%s", str);
		int r = Trie::search(&root, str, strlen((char*)str));
		printf("%d copies found\n", r);
	}

}

void suffix_Experiment()
{
	Trie::Node root;
	Trie::init(&root);

	Trie::add_Suffix(&root, (unsigned char*)"apple", strlen("apple"));
	Trie::add_Suffix(&root, (unsigned char*)"apricot", strlen("apricot"));
	Trie::add_Suffix(&root, (unsigned char*)"apron", strlen("apron"));
	Trie::add_Suffix(&root, (unsigned char*)"application", strlen("application"));

	for (;;)
	{
		static unsigned char str[512];
		printf("enter a prefix >");
		scanf("%s", str);
		int r = Trie::search(&root, str, strlen((char*)str));
		printf("%d copies found\n", r);
	}

}

void disassembly_Experiment()
{
	int subsequence_length = 3;
	Trie::Node root;
	Trie::init(&root);

	Trie::add_Disassembly(&root, (unsigned char*)"apple", strlen("apple"), subsequence_length);
	Trie::add_Disassembly(&root, (unsigned char*)"apricot", strlen("apricot"), subsequence_length);
	Trie::add_Disassembly(&root, (unsigned char*)"apron", strlen("apron"), subsequence_length);
	Trie::add_Disassembly(&root, (unsigned char*)"application", strlen("application"), subsequence_length);

	for (;;)
	{
		static unsigned char str[512];
		printf("enter a prefix >");
		scanf("%s", str);
		int r = Trie::search(&root, str, strlen((char*)str));
		printf("%d copies found\n", r);
	}

}

void generate_Words(int buffer_size)
{
	int subsequence_length = 8;
	Trie::Node root;
	Trie::init(&root);

	//place the txt file next to your main.cpp or provide absolute path
	FILE *f = fopen("dracula.txt", "rb");
	fseek(f, 0, SEEK_END);
	int filesize = ftell(f);
	fseek(f, 0, SEEK_SET);
	unsigned char *data = (unsigned char*)malloc(filesize+1);
	fread(data, 1, filesize, f);
	data[filesize] = 0;
	fclose(f);

	Trie::add_Disassembly(&root, data, filesize, subsequence_length);

	unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char)*buffer_size);

	for (int i = 0; i < subsequence_length; i++) buffer[i] = data[i];

	int position_in_buffer = 0;
	for (int position_in_buffer = 0; position_in_buffer < buffer_size - subsequence_length; position_in_buffer++)
	{
		char c = Trie::pick_Random(&root, &buffer[position_in_buffer], subsequence_length - 1);
		buffer[position_in_buffer + subsequence_length - 1] = c;
	}
	buffer[buffer_size] = 0;

	FILE *f_out = fopen("generated_out.txt", "w+");
	fwrite(buffer, 1, buffer_size, f_out);
	fclose(f_out);
}

int main()
{
	srand(time(0));

	//PICK ONE

	//words_Only_Experiment();
	//suffix_Experiment();
	//disassembly_Experiment();
	generate_Words(20000);

	printf("done!\n");
	
	return 0;
}