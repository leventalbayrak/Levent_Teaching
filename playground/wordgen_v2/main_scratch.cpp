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
	int **table;//index by n_alphabet*n_span, when indexing distance cannot be 0, dist -> 1..n_span
	int n_alphabet = 256;
	int n_span;
	int *tmp_sum;

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
		tmp_sum = new int[n_alphabet];

		//cant index 0
		table = new int*[n_alphabet*(n_span+1)];
		for (int i = 0; i < n_alphabet*(n_span+1); i++)
		{
			table[i] = new int[n_alphabet];
			memset(table[i], 0, sizeof(int)*n_alphabet);
		}
		
		printf("parse\n");
		for (int i = 0; i <= size - n_span - 1; i++)
		{
			for (int j = 0; j < n_span; j++)
			{
				int dist = n_span-j;
				unsigned char c = src[i + j];
				int outcome = src[i + n_span];
				table[dist*c][outcome]++;
			}
		}
	}

	unsigned char predict_Next(unsigned char *str)
	{
		memset(tmp_sum, 0, sizeof(int)*n_alphabet);
		int total_sum = 0;
		for (int i = 0; i < n_span; i++)
		{
			int dist = n_span - i;
			unsigned char c = str[i];
			int index = dist*c;
			for (int j = 0; j < n_alphabet; j++)
			{
				tmp_sum[j] += table[index][j];
				total_sum += table[index][j];
			}
		}

		//cant predict anything at all
		if (total_sum == 0)
		{
			return 0;
		}

		int s = Random::rand_UINT() % total_sum;
		int t = 0;
		for (unsigned char i = 0; i < n_alphabet; i++)
		{
			t += tmp_sum[i];
			if (t > s)
			{
				return i;
			}
		}
		return 0;
	}

	void print_Next_Freq(unsigned char *str)
	{
		memset(tmp_sum, 0, sizeof(int)*n_alphabet);
		int total_sum = 0;
		for (int i = 0; i < n_span; i++)
		{
			int dist = n_span - i;
			unsigned char c = str[i];
			int index = dist*c;
			for (int j = 0; j < n_alphabet; j++)
			{
				tmp_sum[j] += table[index][j];
				total_sum += table[index][j];
			}
		}

		for (unsigned char c = 'a'; c <= 'z'; c++)
		{
			printf("%c -> %u\n", c, tmp_sum[c]);
		}
		printf("total sum %u\n", total_sum);
	}
}

int t1(char *filename, int n_span)
{
	printf("init\n");
	Random::init(0);

	unsigned int size = 0;
	unsigned char *src = new unsigned char[1000000];
	WG::load(src, size, filename);
	WG::init(src, size, n_span);
	
	static unsigned char str[10000];

	printf("%s\n",src);
	
	printf("predict n_span = %d\n",n_span);

	for (int i = 0; i < n_span; i++)
	{
		str[i] = src[i];
		//printf("%c -> %c\n", str[i], src[i]);
		printf("%c", src[i]);
	}

	int k = WG::n_span;
	for (int i = 0; i < 100; i++)
	{
		str[k] = WG::predict_Next(&str[k - WG::n_span]);
		if (str[k] != 0)
		{
			if (str[k] < 'a' || str[k] > 'z')
			{
				int v = str[k];
				printf("\nERROR: %c -> %d\n", str[k], v);
			}
		}
		printf("%c", str[k]);
		k++;
	}
	getchar();

	return 0;
}

int t2()
{
	printf("init\n");
	Random::init(0);

	unsigned int size = 0;
	unsigned char *str = (unsigned char*)"levent";
	WG::init(str, strlen((char*)str), 3);
	
	WG::print_Next_Freq((unsigned char*)"lev");
	
	int e_count = 0;
	int t_count = 0;
	for (int i = 0; i < 5000; i++)
	{
		unsigned char c = WG::predict_Next((unsigned char*)"lev");
		//printf("%c", c);
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
	for (int i = 0; i < length-n_span; i++)
	{
		tmp[k] = WG::predict_Next(&tmp[k - WG::n_span]);
		if (tmp[k] == 8 || tmp[k]==127)
		{
			int v = tmp[k];
			printf("\nERROR: %c -> %d\n", tmp[k], v);
		}
		fprintf(f, "%c", tmp[k]);
		k++;
	}
	fclose(f);
	fflush(f);

	system("gvim log.txt");

	return 0;
}


int main()
{
	t1_2("enwik8.txt", 5, 10000);

	return 0;
}