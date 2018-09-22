#pragma once
#pragma warning(disable:4996)

#include <time.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
using namespace std;

namespace Generator
{
	namespace Utility
	{
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

	namespace internal
	{
		namespace Random
		{
			mt19937 engine;
			unsigned int _max = 0;
			static void init(unsigned int _seed)
			{
				if (_seed == 0) _seed = time(0);
				engine.seed(_seed);
				_max = UINT_MAX;// engine.max();
			}
			inline unsigned int rand_UINT() { return engine(); }
			inline double rand_DOUBLE() { return (double)engine() / _max; }
		}

		namespace Node
		{
			struct Node
			{
				unsigned int sum;
				unsigned char *edge;
				unsigned char n_children;
				Node *children;
				unsigned char size;
			};

			void add_Nmer(Node *root, const char *nmer, unsigned int nmer_size)
			{
				Node *current = root;
				current->sum++;
				for (unsigned int i = 0; i < nmer_size; i++)
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
						current->edge[current->n_children] = nmer[i];
						current->n_children++;
						current = &current->children[current->n_children - 1];
					}
					else
					{
						int which_edge = -1;
						for (int j = 0; j < current->n_children; j++)
						{
							if (current->edge[j] == nmer[i])
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
<<<<<<< HEAD
								current->size += current->size << 1;
=======
								current->size += current->size >> 1;
>>>>>>> development
								current->children = (Node*)realloc(current->children, sizeof(Node)*current->size); assert(current->children);
								current->edge = (unsigned char*)realloc(current->edge, sizeof(unsigned char)*current->size); assert(current->edge);
							}

							current->children[current->n_children] = {};
							current->children[current->n_children].sum++;
							current->edge[current->n_children] = nmer[i];

							current->n_children++;
							current = &current->children[current->n_children - 1];
						}

					}
				}
			}

			void add_Str(Node *root, const char *str, unsigned int nmer_size)
			{
				unsigned int len = strlen(str);
				if (len < nmer_size) return;
				//+1 makes NULL terminator be treated as a valid character
				for (unsigned int i = 0; i <= len - nmer_size + 1; i++)
				{
					add_Nmer(root, &str[i], nmer_size);
				}
			}

			unsigned char random_Edge(const Node *root, const char *word, unsigned int depth, unsigned int nmer_size)
			{
				//len cannot be more than nmer_size-1
				if (depth >= nmer_size) return 0;

				Node *current = (Node*)root;
				for (unsigned int i = 0; i < depth; i++)
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
				unsigned int s = z * current->sum;
				unsigned int t = 0;
				for (int i = 0; i < current->n_children; i++)
				{
					t += current->children[i].sum;
					if (t > s)
					{
						next = i;
						break;
					}
				}

				if (next < 0)
				{
					return 0;
				}
				return current->edge[next];
			}

			void random_Nmer(char *leaf_path, unsigned int nmer_size, const Node *root)
			{
				unsigned int k = 0;
				Node *current = (Node*)root;
				for (unsigned int i = 0; i < nmer_size; i++)
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

			void random_Str(char *str, unsigned int max_length, const Node *root,unsigned int depth, unsigned int nmer_size)
			{
				//depth cannot be more than nmer_size
				if (depth > nmer_size) return;

				random_Nmer(str, nmer_size, root);
				unsigned int k = nmer_size;
				for (;;)
				{
					unsigned char c = random_Edge(root, &str[k + 1 - nmer_size], nmer_size - 1, nmer_size);
					str[k++] = c;
					if (c == 0) return;
					if (k == max_length)
					{
						str[max_length] = 0;
						return;
					}
				}
			}
		}
	}

	struct Generator
	{
		internal::Node::Node root;
		int nmer_size;
	};

	//trie will be built out of fixed nmer size strings
	void init(Generator *g, int nmer_size)
	{
		g->nmer_size = nmer_size;
		g->root = {};

		internal::Random::init(0);
	}

	//string will be disassembled into nmer_size substrings and added to trie
	void add_Str(Generator *g, const char *str, unsigned int len)
	{
		if (len < g->nmer_size) return;
		//+1 makes NULL terminator be treated as a valid character
		for (unsigned int i = 0; i <= len - g->nmer_size + 1; i++)
		{
			internal::Node::add_Nmer(&g->root, &str[i], g->nmer_size);
		}
	}
	
	//can pick any depth between 1 and nmer_size-1
	void generate(char *str, unsigned int max_length, const Generator *g, int depth)
	{
		internal::Node::random_Str(str, max_length, &g->root, depth, g->nmer_size);
	}
}