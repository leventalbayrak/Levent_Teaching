//idea is to:
//make a fixed length nmer trie, length=N
//get probability of next characters given length-1 characters
//up to here, its just markov chain, with no context
//make another table which contains character counts in a fixed size window (much larger than nmer length),
//each single character takes freq values across all windows
//associate these frequencies with the next character at the end of the window

//now you have 2 probabilities, do math, figure out next character based on trie+this
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <random>
#include <time.h>
using namespace std;

namespace Globals
{
	const int n_alphabet = 256;
}

namespace Util
{
	void load_File(unsigned char *&buffer, unsigned int &size, char *filename)
	{
		printf("read file\n");
		FILE *f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (unsigned char*)malloc(size + 1); assert(buffer);
		fread(buffer, 1, size + 1, f);//strtok wont work without a NULL terminator
		buffer[size] = 0;
		fclose(f);
		printf("file size: %u\n", size);
	}
}

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

//window freq
namespace WF
{
	struct WF
	{
		int ***table;//index = [unsigned char][freq][profile]
		int **char_freq_window_sum;
		int *char_freq_ending_sum;//char found at the end of window
		int window_size;
	};

	void init(WF *w, int window_size)
	{
		w->window_size = window_size;

		w->table = new int**[Globals::n_alphabet]; assert(w->table);
		w->char_freq_window_sum = new int*[Globals::n_alphabet]; assert(w->char_freq_window_sum);
		w->char_freq_ending_sum = new int[Globals::n_alphabet]; assert(w->char_freq_ending_sum);
		memset(w->char_freq_ending_sum, 0, sizeof(int)*Globals::n_alphabet);
		for (int i = 0; i < Globals::n_alphabet; i++)
		{
			w->char_freq_window_sum[i] = new int[w->window_size + 1]; assert(w->char_freq_window_sum[i]);
			memset(w->char_freq_window_sum[i], 0, sizeof(int)*(w->window_size + 1));

			w->table[i] = new int*[w->window_size + 1]; assert(w->table[i]);
			for (int j = 0; j < w->window_size + 1; j++)
			{
				w->table[i][j] = new int[Globals::n_alphabet]; assert(w->table[i][j]);
				memset(w->table[i][j], 0, sizeof(int)*Globals::n_alphabet);
			}
		}
	}

	void add_Disassembly(WF *w, unsigned char *str, int len)
	{
		int tmp[256];
		
		for (int i = 0; i <= len - w->window_size - 1; i++)
		{
			memset(tmp, 0, sizeof(int) * Globals::n_alphabet);
			for (int j = 0; j < w->window_size; j++)
			{
				unsigned char c = str[i + j];

				tmp[c]++;
			}

			unsigned char window_ends_with = str[i + w->window_size];
			w->char_freq_ending_sum[window_ends_with]++;
			for (int j = 0; j < Globals::n_alphabet; j++)
			{
				if (tmp[j] == 0) continue;
				w->char_freq_window_sum[j][tmp[j]]++;
				w->table[j][tmp[j]][window_ends_with]++;
			}
		}
	}

	void get_Window_Ending_Prob(double *prob, WF *w, unsigned char *str, int len)
	{
		int freq[256];
		memset(freq, 0, sizeof(int)*Globals::n_alphabet);
	
		for (int i = 0; i < len; i++)
		{
			unsigned char window_c = str[i];
			freq[window_c]++;
		}

		for (int i = 0; i < Globals::n_alphabet; i++) prob[i] = 1.0;
		
		for (int i = 0; i < Globals::n_alphabet; i++)
		{
			if (freq[i] == 0) continue;
			for (int j = 0; j < Globals::n_alphabet; j++)
			{
				if (w->char_freq_window_sum[i][freq[i]] == 0) continue;
				prob[j] *= 1.0-(double)w->table[i][freq[i]][j]/w->char_freq_window_sum[i][freq[i]];
			}
		}

		double sum = 0.0;
		for (int i = 0; i < Globals::n_alphabet; i++)
		{
			prob[i] = 1.0 - prob[i];
			sum += prob[i];
		}

		if (sum == 0.0)
		{
			//THIS SHOULD NEVER HAPPEN (?)
			printf("WINDOW this should not happen\n");
			for (int i = 0; i < len; i++)
			{
				printf("%d ", str[i]);
			}
			printf("\n");
			getchar();
			return;
			assert(0);
		}

		for (int i = 0; i < Globals::n_alphabet; i++)
		{
			prob[i] /= sum;
		}
	}
}

namespace WT
{
	namespace Node
	{
		struct Node
		{
			int sum;
			Node *branches[Globals::n_alphabet];
		};

		void init(Node *n)
		{
			n->sum = 0;
			memset(n->branches, 0, sizeof(Node*)*Globals::n_alphabet);
		}

		void add(Node *root, unsigned char *str, int len)
		{
			Node *n = root;
			for (int i = 0; i < len; i++)
			{
				unsigned char c = str[i];
				if (n->branches[c] == NULL)
				{
					n->branches[c] = new Node; assert(n->branches[c]);
					init(n->branches[c]);
				}
				n = n->branches[c];
				n->sum++;
			}
		}

		void get_Branch_Prob(double *prob, Node *root, unsigned char *str, int len)
		{
			memset(prob, 0, sizeof(double)*Globals::n_alphabet);
			Node *n = root;
			for (int i = 0; i < len; i++)
			{
				unsigned char c = str[i];
				if (n->branches[c] == NULL)
				{
					//THIS SHOULD NEVER HAPPEN
					printf("TREE this should not happen\n");
					for (int i = 0; i < len; i++)
					{
						printf("%d ", str[i]);
					}
					printf("\n");
					getchar();
					return;
					assert(NULL);
				}
				n = n->branches[c];
			}
			for (int i = 0; i < Globals::n_alphabet; i++)
			{
				if (n->branches[i] == NULL) continue;
				prob[i] = (double)n->branches[i]->sum / n->sum;
				
			}
		}
	}

	struct Trie
	{
		Node::Node root;
	};

	void init(Trie *t)
	{
		Node::init(&t->root);
	}

	void add_Single(Trie *t, unsigned char *str, int len)
	{
		Node::add(&t->root, str, len);
		t->root.sum++;
	}

	void add_Disassembly(Trie *t, unsigned char *str, int len, int nmer_len)
	{
		//FILE *f = fopen("log.txt", "w+");
		for (int i = 0; i <= len - nmer_len; i++)
		{
			//fprintf(f,"%d\n", i);
			add_Single(t, &str[i], nmer_len);
			//fprintf(f,"done\n", i);
		}
		//fclose(f);
	}

	void get_Branch_Prob(double *prob, Trie *t, unsigned char *str, int len)
	{
		Node::get_Branch_Prob(prob, &t->root, str, len);
	}
}


void t0()
{
	char *buffer = "abcdef washingmachine, xyzuvp washington dc";
	//char *context_window = "abcdef washing";
	char *context_window = "xyzuvp washing";
	int nmer_len = 3;
	int context_window_size = strlen(context_window);
	char *nmer = &context_window[context_window_size - nmer_len + 1];

	unsigned int buffer_size = strlen(buffer);
	
	printf("build trie\n");
	WT::Trie t;
	WT::init(&t);

	WT::add_Disassembly(&t, (unsigned char*)buffer, buffer_size, nmer_len);

	printf("build context window\n");
	WF::WF w;
	WF::init(&w, context_window_size);

	WF::add_Disassembly(&w, (unsigned char*)buffer, buffer_size);

	static double tmp_prob_A[256];
	static double tmp_prob_B[256];
	static double tmp_prob_C[256];

	WT::get_Branch_Prob(tmp_prob_A, &t, (unsigned char*)nmer, strlen(nmer));
	
	WF::get_Window_Ending_Prob(tmp_prob_B, &w, (unsigned char*)context_window, context_window_size);

	printf("context window: %s\n", context_window);
	printf("nmer: %s\n", nmer);
	for (int i = 'a'; i <= 'z'; i++)
	{
		printf("%c %f %f -> %f\n",i, tmp_prob_A[i], tmp_prob_B[i], tmp_prob_A[i]*tmp_prob_B[i]);
	}
	getchar();
}

#define DEBUG 0
void t1()
{

	char *filename = "test.txt";
	//char *filename = "trump.txt";
	//char *filename = "input.txt";
	//char *filename = "enwik8.txt";
	//char *filename = "main_scratch_nmer_with_context.cpp";
	const int nmer_len = 5;
	const int context_window_size = 500;
	int gen_len = 10000;

	char output_filename[256];
	sprintf(output_filename, "%s_OUT.txt", filename);

	unsigned char *buffer = NULL;
	unsigned int buffer_size = 0;

	Random::init(0);

	printf("loading file\n");
	Util::load_File(buffer, buffer_size, filename);

	printf("build trie\n");
	WT::Trie t;
	WT::init(&t);

	WT::add_Disassembly(&t, buffer, buffer_size, nmer_len);

	printf("build context window\n");
	WF::WF w;
	WF::init(&w,context_window_size);

	WF::add_Disassembly(&w,buffer,buffer_size);

	static double tmp_prob_A[256];
	static double tmp_prob_B[256];
	static double tmp_prob_C[256];
	static int tmp_freq[256];

	unsigned char *tmp_buffer = new unsigned char[gen_len * 2];
	unsigned int position_in_tmp_buffer = 0;
	for (int i = 0; i < context_window_size; i++)
	{
		tmp_buffer[i] = buffer[i];
	}
	position_in_tmp_buffer = context_window_size;

	char logfilename[256];
	sprintf(logfilename, "%s_DETAIL.txt", output_filename);
	FILE *f_log = fopen(logfilename, "w+");

	printf("Generate\n");
	for (; position_in_tmp_buffer < gen_len; position_in_tmp_buffer++)
	{
		unsigned char nmer[nmer_len+1];
		for (int i = 0; i < nmer_len - 1; i++)
		{
			nmer[i] = tmp_buffer[position_in_tmp_buffer - nmer_len + 1 + i];
		}
		nmer[nmer_len - 1] = 0;
		
		WT::get_Branch_Prob(tmp_prob_A, &t, nmer, nmer_len - 1);
		
		unsigned char context_window[context_window_size+1];
		for (int i = 0; i < context_window_size; i++)
		{
			context_window[i] = tmp_buffer[position_in_tmp_buffer - context_window_size + i];
		}
		context_window[context_window_size] = 0;

		WF::get_Window_Ending_Prob(tmp_prob_B, &w, context_window, context_window_size);

		for (int i = 0; i < Globals::n_alphabet; i++)
		{
			//tmp_prob_C[i] = tmp_prob_A[i]*tmp_prob_B[i];
			tmp_prob_C[i] = tmp_prob_A[i];
		}

		double sum = 0.0;
		for (int i = 0; i < Globals::n_alphabet; i++) sum += tmp_prob_C[i];
		for (int i = 0; i < Globals::n_alphabet; i++) tmp_prob_C[i] /= sum;

		tmp_buffer[position_in_tmp_buffer] = '$';
		double s = Random::rand_DOUBLE();
		assert(s <= 1.0);
		double t = 0;
		for (int i = 0; i < Globals::n_alphabet; i++)
		{
			if (tmp_prob_C[i] > 1.0)
			{
				printf("ERROR: %f\n", tmp_prob_C[i]);
				getchar();
			}
			t += tmp_prob_C[i];
			if (t >= s)
			{
				tmp_buffer[position_in_tmp_buffer] = i;
				break;
			}
		}

		fprintf(f_log, "|context:%s|nmer:%s|select:%c|\n", context_window, nmer, tmp_buffer[position_in_tmp_buffer]);

	}

	fclose(f_log);

	FILE *f = fopen(output_filename, "w+");
	fwrite(tmp_buffer, 1, gen_len, f);
	fclose(f);
	
	static char cmd[512];
	sprintf(cmd, "gvim %s", output_filename);
	system(cmd);
	//sprintf(cmd, "gvim %s", logfilename);
	//system(cmd);
	getchar();
}

int main()
{
	t1();

	return 0;
}