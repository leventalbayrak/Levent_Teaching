/*
what is terminal?
are generated words independent or is there 1 piece of text?
*/

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
				unsigned char n_nodes;
				unsigned char size;
				Node *nodes;
			};

			void add_Nmer(Node *root, const unsigned char *nmer, int nmer_size)
			{
				Node *current = root;	
				for (int i = 0; i < nmer_size; i++)
				{
					
					if (current->size == 0)
					{
						assert(current->nodes == NULL);
						assert(current->n_nodes == 0);
						assert(current->edge == NULL);

						current->n_nodes = 0;
						current->size = 64;
						current->nodes = (Node*)malloc(sizeof(Node)*current->size); assert(current->nodes);
						current->edge = (unsigned char*)malloc(sizeof(unsigned char)*current->size); assert(current->edge);

						current->nodes[current->n_nodes] = {};
						current->edge[current->n_nodes] = nmer[i];
						current->n_nodes++;
						current = &current->nodes[current->n_nodes - 1];
						current->sum++;
					}
					else
					{
						int which_edge = -1;
						for (int j = 0; j < current->n_nodes; j++)
						{
							if (current->edge[j] == nmer[i])
							{
								which_edge = j;
								break;
							}
						}

						if (which_edge >= 0)
						{
							current = &current->nodes[which_edge];
							current->sum++;
						}
						else
						{
							if (current->n_nodes >= current->size)
							{
								assert(current->size != 0);
								current->size += current->size >> 1;
								current->nodes = (Node*)realloc(current->nodes, sizeof(Node)*current->size); assert(current->nodes);
								current->edge = (unsigned char*)realloc(current->edge, sizeof(unsigned char)*current->size); assert(current->edge);
							}

							current->nodes[current->n_nodes] = {};
							current->edge[current->n_nodes] = nmer[i];
							current->n_nodes++;
							current = &current->nodes[current->n_nodes - 1];
							current->sum++;
						}

					}
				}
			}

			void add_Str(Node *root, const unsigned char *str, int len, int nmer_size)
			{
				if (len < nmer_size) return;
				//+1 makes NULL terminator be treated as a valid character
				for (int i = 0; i <= len - nmer_size; i++)
				{
					add_Nmer(root, &str[i], nmer_size);
				}
				//add last nmer_size-1 plus 0 terminator
				add_Nmer(root, &str[len - nmer_size + 1], nmer_size);
			}

			unsigned char random_Edge(const Node *root, const unsigned char *nmer_prefix, int depth, int nmer_size)
			{
				//len cannot be more than nmer_size-1
				if (depth >= nmer_size) return 0;

				Node *current = (Node*)root;
				for (int i = 0; i < depth; i++)
				{
					char c = nmer_prefix[i];
					int which_edge = -1;
					for (int j = 0; j < current->n_nodes; j++)
					{
						if (current->edge[j] == c)
						{
							which_edge = j;
							break;
						}
					}

					if (which_edge >= 0)
					{
						current = &current->nodes[which_edge];
					}
					else
					{
						return 0;
					}
				}

				int next = -1;
				double z = Random::rand_DOUBLE();
				int s = z * current->sum;
				int t = 0;
				for (int i = 0; i < current->n_nodes; i++)
				{
					t += current->nodes[i].sum;
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

			void random_Nmer(unsigned char *leaf_path, int nmer_size, const Node *root)
			{
				unsigned int k = 0;
				Node *current = (Node*)root;
				for (int i = 0; i < nmer_size; i++)
				{
					int next = -1;
					unsigned int s = Random::rand_DOUBLE()*current->sum;
					unsigned int t = 0;
					for (int j = 0; j < current->n_nodes; j++)
					{
						t += current->nodes[j].sum;
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
						current = &current->nodes[next];
					}
				}

				leaf_path[k] = 0;
			}

			void random_Str(unsigned char *str, int max_length, const Node *root,int depth, int nmer_size)
			{
				//depth cannot be more than nmer_size
				if (depth > nmer_size) return;

				random_Nmer(str, nmer_size, root);
				int k = nmer_size;
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
	void add_Str(Generator *g, const unsigned char *str, int len)
	{
		if (len < g->nmer_size) return;
		//+1 makes NULL terminator be treated as a valid character
		for (unsigned int i = 0; i <= len - g->nmer_size + 1; i++)
		{
			g->root.sum++;
			internal::Node::add_Nmer(&g->root, &str[i], g->nmer_size);
		}
	}
	
	//can pick any depth between 1 and nmer_size-1
	void generate(unsigned char *str, int max_length, const Generator *g, int depth)
	{
		internal::Node::random_Str(str, max_length, &g->root, depth, g->nmer_size);
	}
}