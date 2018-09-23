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
	int **table;//index by n_alphabet*n_span
	int n_alphabet = 256;
	int n_span = 10;
	int *tmp_sum;

	void init(char *str, char *filename)
	{
		printf("read file\n");
		FILE *f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		unsigned int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		unsigned char *buffer = (unsigned char*)malloc(size + 1); assert(buffer);
		fread(buffer, 1, size + 1, f);//strtok wont work without a NULL terminator
		buffer[size] = 0;
		fclose(f);
		printf("file size: %u\n", size);

		printf("alloc\n");
		tmp_sum = new int[n_alphabet];
		table = new int*[n_alphabet*n_span];
		for (int i = 0; i < n_alphabet*n_span; i++)
		{
			table[i] = new int[n_alphabet];
			memset(table[i], 0, sizeof(int)*n_alphabet);
		}
		
		printf("parse\n");
		for (int i = 0; i <= size - n_span - 1; i++)
		{
			//printf("%d\n", i);
			for (int j = 0; j < n_span; j++)
			{
				int dist = n_span-j-1;
				unsigned char c = buffer[i + j];
				int outcome = buffer[i + n_span];
				table[dist*c][outcome]++;
			}
		}

		unsigned int pos = Random::rand_UINT() % (size - n_span);
		for (int i = 0; i < n_span; i++)
		{
			str[i] = buffer[pos + i];
		}

	}

	unsigned char predict_Next(char *str)
	{
		memset(tmp_sum, 0, sizeof(int)*n_alphabet);
		int total_sum = 0;
		for (int i = 0; i < n_span; i++)
		{
			int dist = n_span - i - 1;
			unsigned char c = str[i];
			for (int j = 0; j < n_alphabet; j++)
			{
				tmp_sum[j] += table[dist*c][j];
				total_sum += table[dist*c][j];
			}
		}

		double z = Random::rand_DOUBLE();
		int s = z * total_sum;
		int t = 0;
		for (int i = 0; i < n_alphabet; i++)
		{
			t += tmp_sum[i];
			if (t > s)
			{
				return i;
			}
		}
		return 0;
	}

}


int main()
{
	printf("init\n");
	Random::init(0);

	char *str = new char[1000000];
	WG::init(str, (char*)"ALL_CONCATENATED.txt");
	str[WG::n_span] = 0;
	
	int k = WG::n_span;
	printf("predict\n");
	for (int i = 0; i < 1000; i++)
	{
		str[k + 1] = WG::predict_Next(&str[k-WG::n_span]);
		printf("%c",str[k+1]);
		k++;
	}
	getchar();

	return 0;
}