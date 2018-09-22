#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <random>
#include <time.h>
using namespace std;

namespace WG
{
	struct Node
	{
		unsigned int sum;
		unsigned char *edge;
		unsigned char n_children;
		Node *children;
		unsigned char size;
	};

	namespace Random
	{
		mt19937 engine;
		unsigned int _max = 0;
		static void init(unsigned int _seed)
		{
			if (_seed == 0) _seed = time(0);
			engine.seed(_seed);
			_max = engine.max();
		}
		inline unsigned int rand_UINT() { return engine(); }
		inline double rand_DOUBLE() { return (double)engine() / engine.max(); }
	}

	void add(Node *root, const char *word, unsigned int len)
	{
		Node *current = root;
		current->sum++;
		for (unsigned int i = 0; i < len; i++)
		{
			if (current->edge == NULL)
			{
				current->n_children = 0;
				current->size = 4;
				current->children = (Node*)malloc(sizeof(Node)*current->size); assert(current->children);
				current->edge = (unsigned char*)malloc(sizeof(unsigned char)*current->size); assert(current->edge);
				
				memset(current->children, 0, sizeof(Node)*current->size);
				memset(current->edge, 0, sizeof(unsigned char)*current->size);

				current->children[current->n_children] = {};
				current->children[current->n_children].sum++;
				current->edge[current->n_children] = word[i];
				current->n_children++;
				current = &current->children[current->n_children - 1];
			}
			else
			{
				int which_edge = -1;
				for (int j = 0; j < current->n_children; j++)
				{
					if (current->edge[j] == word[i])
					{
						which_edge = j;
						break;
					}
				}

				if (which_edge >= 0)
				{
					current->children[which_edge].sum++;
					current = &current->children[which_edge];
				}
				else
				{
					if (current->n_children >= current->size)
					{
						current->size += current->size << 1;
						current->children = (Node*)realloc(current->children, sizeof(Node)*current->size); assert(current->children);
						current->edge = (unsigned char*)realloc(current->edge, sizeof(unsigned char)*current->size); assert(current->edge);
					}

					current->children[current->n_children] = {};
					current->children[current->n_children].sum++;
					current->edge[current->n_children] = word[i];

					current->n_children++;
					current = &current->children[current->n_children - 1];
				}

			}
		}
	}

	void add(Node *root, char **words, unsigned int n_words, unsigned int nmer_size)
	{
		for (unsigned int i = 0; i < n_words; i++)
		{
			unsigned int len = strlen(words[i]);
			if (len < nmer_size) continue;
			//+1 makes NULL terminator be treated as a valid character
			for (unsigned int j = 0; j <= len - nmer_size + 1; j++)
			{
				add(root, &words[i][j], nmer_size);
			}
		}
	}
	
	unsigned char random_Edge(Node *root, const char *word, unsigned int len)
	{
		Node *current = root;
		for (unsigned int i = 0; i < len; i++)
		{
			char c = word[i];
			int which_edge = -1;
			for (int j = 0; j < current->n_children; j++)
			{
				if (current->edge[j] == c)
				{
					which_edge = j;
					break;
				}
			}

			if (which_edge >= 0)
			{
				current = &current->children[which_edge];
			}
			else
			{
				return 0;
			}
		}

		int next = -1;
		double z = Random::rand_DOUBLE();
		unsigned int s = z*current->sum;
		unsigned int t = 0;
		for (int j = 0; j < current->n_children; j++)
		{
			t += current->children[j].sum;
			if (t > s)
			{
				next = j;
				break;
			}
		}

		if (next < 0)
		{
			return 0;
		}
		return current->edge[next];
	}


	void random_Leaf(char *leaf_path, unsigned int len, Node *root)
	{
		unsigned int k = 0;
		Node *current = root;
		for (unsigned int i = 0; i < len; i++)
		{
			int next = -1;
			unsigned int s = Random::rand_DOUBLE()*current->sum;
			unsigned int t = 0;
			for (int j = 0; j < current->n_children; j++)
			{
				t += current->children[j].sum;
				if (t > s)
				{
					next = j;
					break;
				}
			}

			if (next < 0)
			{
				break;
			}
			else
			{
				leaf_path[k++] = current->edge[next];
				current = &current->children[next];
			}
		}

		leaf_path[k] = 0;
	}

	void random_Word(char *word, Node *root, unsigned int len)
	{
		random_Leaf(word, len, root);
		unsigned int k = len;
		for (;;)
		{
			unsigned char c = random_Edge(root, &word[k-len], len);
			word[k++] = c;
			if (c == 0) return;
		}
	}

	void tokenize(char **&words, unsigned int &n_words, char *buffer, const char *delim)
	{
		unsigned int words_size = 1024;
		n_words = 0;
		words = (char**)malloc(sizeof(char*)*words_size); assert(words);

		char *s = strtok(buffer, delim);
		while (s != NULL)
		{
			if (n_words >= words_size)
			{
				words_size += words_size >> 1;
				words = (char**)realloc(words, sizeof(char*)*words_size); assert(words);
			}

			words[n_words++] = s;
			s = strtok(NULL, delim);
		}
	}

	void load_Dictionary(char **&words, unsigned int &n_words, const char *filename)
	{
		FILE *f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		unsigned int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		char *buffer = (char*)malloc(size + 1); assert(buffer);
		fread(buffer, 1, size + 1, f);//strtok wont work without a NULL terminator
		buffer[size] = 0;
		tokenize(words, n_words, buffer, (char*)"\n\r\t ");
		fclose(f);
	}

}

int main()
{
	unsigned int nmer_size = 3;
	unsigned int n_gen = 50000000;
	char *filename_dictionary = (char*)"moby\\ALL_CONCATENATED.TXT";
	char *filename_gen_output = (char*)"generated_words_3mer_50m.txt";

	WG::Random::init(0);
	char **words = NULL;
	unsigned int n_words = 0;
	WG::load_Dictionary(words, n_words, filename_dictionary);

	WG::Node root = {};
	WG::add(&root, words, n_words, nmer_size);

	FILE *f = fopen(filename_gen_output, "w+");
	static char word[1024];
	unsigned int completed_counter = 0;
	for (unsigned int i = 0; i < n_gen; i++)
	{
		if (++completed_counter > n_gen / 100)
		{
			printf("completed %.2f\n", 100.0*i / n_gen);
			completed_counter = 0;
		}
		WG::random_Word(word, &root, nmer_size - 1);
		fprintf(f, "%s\n", word);
	}
	fclose(f);
	
	return 0;
}