#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <random>
#include <time.h>
using namespace std;

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

namespace WG
{
	int ***table;//index by src char, distance, target char
	int **table_row_sum;//index by src char, distance
	int *n_terminal;//alphabet char was terminal
	int n_total_terminal;
	int n_alphabet = 256;
	int n_span;
	int *tmp_sum;
	double *tmp_prob;

	void load(unsigned char *&buffer, unsigned int &size, char *filename)
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

	void init(unsigned char *src, unsigned int size, int _n_span)
	{
		n_span = _n_span;

		printf("alloc\n");
		
		tmp_prob = new double[n_alphabet];

		//cant index 0
		n_terminal = new int[n_alphabet];
		memset(n_terminal, 0, sizeof(int)*n_alphabet);

		table_row_sum = new int*[n_alphabet];
		table = new int**[n_alphabet];
		for (int i = 0; i < n_alphabet; i++)
		{
			table[i] = new int*[n_span];
			for (int j = 0; j < n_span; j++)
			{
				table[i][j] = new int[n_alphabet];
				memset(table[i][j], 0, sizeof(int)*n_alphabet);
			}	

			table_row_sum[i] = new int[n_span];
			memset(table_row_sum[i], 0, sizeof(int)*n_span);
		}
		n_total_terminal = 0;
		
		printf("parse\n");
		for (int i = 0; i <= size - n_span - 1; i++)
		{
			for (int j = 0; j < n_span; j++)
			{
				int dist = n_span - j - 1;
				unsigned char c = src[i + j];
				int outcome = src[i + n_span];
				table[c][dist][outcome]++;
				table_row_sum[c][dist]++;
				n_terminal[outcome]++;
			}
			n_total_terminal++;
		}
	}

	unsigned char predict_Next_0(unsigned char *str)
	{
		memset(tmp_prob, 0, sizeof(double)*n_alphabet);

		for (int i = 0; i < n_span; i++)
		{
			int dist = n_span - i - 1;
			unsigned char c = str[i];
		
			if (table_row_sum[c][dist] == 0) continue;

			for (int j = 0; j < n_alphabet; j++) tmp_prob[j] = 1.0;

			//probability that c in pos i did not cause these terminals
			for (int j = 0; j < n_alphabet; j++)
			{
				tmp_prob[j] *= (1.0 - ((double)table[c][dist][j] / table_row_sum[c][dist]));
			}
		}

		printf("%s\n", str);
		//probability that one or more c's caused this terminal at their positions
		for (int i = 0; i < n_alphabet; i++)
		{
			tmp_prob[i] = 1.0 - tmp_prob[i];

			if (tmp_prob[i] != 0.0 && i>= 'a' && i <= 'z')
			{
				printf("%c -> %f\n", i, tmp_prob[i]);
			}
		}
		getchar();
		double sum = 0.0;
		for (int i = 0; i < n_alphabet; i++)
		{
			/*if (tmp_prob[i] != 0.0)
			{
				printf("tmp %f\n", tmp_prob[i]);
			}*/
			sum += tmp_prob[i];
		}

		if (sum == 0.0)
		{
			return 0.0;
		}

		//normalize
		for (int i = 0; i < n_alphabet; i++)
		{
			tmp_prob[i] /= sum;
		}

		double s = Random::rand_DOUBLE();
		assert(s <= 1.0);
		double t = 0;
		for (unsigned char i = 0; i < n_alphabet; i++)
		{
			t += tmp_prob[i];
			if (t >= s)
			{
				return i;
			}
		}
		return 0;
	}

	unsigned char predict_Next_1(unsigned char *str)
	{
		double fuzz = 0;// 0.000001;
		memset(tmp_prob, 0, sizeof(double)*n_alphabet);

		for (int i = 0; i < n_span; i++)
		{
			int dist = n_span - i - 1;
			unsigned char c = str[i];

			if (table_row_sum[c][dist] == 0) continue;

			//double alpha = (1.0 / (1.0 + exp(dist)));
			//double alpha = 1.0 / (dist*dist + 1.0);
			double alpha = 1.0;
			
			for (int j = 0; j < n_alphabet; j++)
			{
				double p = (double)table[c][dist][j] / table_row_sum[c][dist];
				if (p != 0)
				{
					p = 1.0 / -log(p);
					tmp_prob[j] += alpha*p;
				}
			}
		}

		double sum = 0.0;
		for (int i = 0; i < n_alphabet; i++)
		{
			sum += tmp_prob[i];
		}

		if (sum == 0.0)
		{
			return 0.0;
		}

		for (int i = 0; i < n_alphabet; i++)
		{
			tmp_prob[i] /= sum;
		}

		double s = Random::rand_DOUBLE();
		assert(s <= 1.0);
		double t = 0;
		for (unsigned char i = 0; i < n_alphabet; i++)
		{
			t += tmp_prob[i];
			if (t >= s)
			{
				return i;
			}
		}
		return 0;
	}

	void print_Next_Prob(unsigned char *str)
	{
		double fuzz = 0.0;
		memset(tmp_prob, 0, sizeof(double)*n_alphabet);

		for (int i = 0; i < n_span; i++)
		{
			int dist = n_span - i - 1;
			unsigned char c = str[i];

			for (int j = 0; j < n_alphabet; j++) tmp_prob[j] = 1.0;

			if (table_row_sum[c][dist] == 0) continue;

			for (int j = 0; j < n_alphabet; j++)
			{
				tmp_prob[j] *= (1.0 - (fuzz + (double)table[c][dist][j] / table_row_sum[c][dist]));
			}

		}

		for (int i = 0; i < n_alphabet; i++)
		{
			tmp_prob[i] = 1.0 - tmp_prob[i];
		}

		double sum = 0.0;
		for (int i = 0; i < n_alphabet; i++)
		{
			sum += tmp_prob[i];
		}
		for (int i = 0; i < n_alphabet; i++)
		{
			tmp_prob[i] /= sum;
		}

		for (unsigned char c = 'a'; c <= 'z'; c++)
		{
			printf("%c -> %f\n", c, tmp_prob[c]);
		}
	}
}

int t2()
{
	printf("init\n");
	Random::init(0);

	unsigned int size = 0;
	unsigned char *str = (unsigned char*)"levent";
	WG::init(str, strlen((char*)str), 3);
	
	WG::print_Next_Prob((unsigned char*)"lev");
	
	int e_count = 0;
	int t_count = 0;
	for (int i = 0; i < 5000; i++)
	{
		unsigned char c = WG::predict_Next_0((unsigned char*)"lev");
		printf("%c", c);
		if (c == 'e')
		{
			e_count++;
		}
		if (c == 't')
		{
			t_count++;
		}
	}
	
	printf("t to e ratio %f\n", (double)t_count / e_count);

	getchar();

	return 0;
}

int t1_2(char *filename, int n_span, int length)
{
	printf("init\n");
	Random::init(0);

	unsigned int size = 0;
	unsigned char *src = NULL;
	WG::load(src, size, filename);
	WG::init(src, size, n_span);

	unsigned char *tmp = new unsigned char[length * 2];

	printf("predict n_span = %d\n", n_span);

	FILE *f = fopen("log.txt", "w+");

	for (int i = 0; i < n_span; i++)
	{
		tmp[i] = src[i];
		fprintf(f, "%c", tmp[i]);
	}

	int k = WG::n_span;
	for (int i = 0; i < length-n_span-1; i++)
	{
		tmp[k] = WG::predict_Next_0(&tmp[k - WG::n_span]);
		fprintf(f, "%c", tmp[k]);
		k++;
		tmp[k] = 0;
	}
	fclose(f);
	fflush(f);

	system("gvim log.txt");

	return 0;
}

int main()
{

	printf("BEGIN\n");
	//t2();
	t1_2((char*)"test.txt", 4, 1000);


	return 0;
}