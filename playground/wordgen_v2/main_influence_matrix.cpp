//idea is to:
//make a 256 by 256 matrix
//
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <random>
#include <time.h>
using namespace std;

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

namespace IM
{
	struct IM
	{
		double m[256][256];//index [influencer char][influenced char]
		int max_dist;//max distance considered
	};

	void init(IM *m)
	{
		for (int i = 0; i < 256; i++)
		{
			memset(m->m[i], 0, sizeof(double) * 256);
		}
	}

	void add(IM *m, unsigned char *str, int len)
	{
		for (int i = 0; i < len; i++)
		{
			unsigned char influencer_c = str[i];
			for (int j = i + 1; j < len; j++)
			{
				unsigned char influenced_c = str[j];
				double influence = (j - i);
				influence = 1.0 / influence;
				m->m[influencer_c][influenced_c] += influence;
			}
		}
	}

	void add_Disassembly(IM *m, unsigned char *str, int len, int substr_len)
	{
		for (int i = 0; i <= len - substr_len; i++)
		{
			add(m, &str[i], substr_len);
		}
	}

	//influence array will contain influence amounts on each character if they came after str's end
	void get_Influence_at_End(double *influence,IM *m, unsigned char *str, int len)
	{
		memset(influence, 0, sizeof(double) * 256);
		for (int i = 0; i < len; i++)
		{
			unsigned char influencer_c = str[i];
			double tmp_influence = (double)(i + 1) / len;
			for (int j = 0; j < 256; j++)
			{
				unsigned char influenced_c = j;
				double v = m->m[influencer_c][influenced_c];
				influence[j] += tmp_influence*v;
			}
		}
	}

	int pick(double *influence)
	{
		double sum = 0.0;
		for (int i = 0; i < 256; i++)
		{
			sum += influence[i];
		}

		double s = sum * Random::rand_DOUBLE();
		double t = 0.0;
		for (int i = 0; i < 256; i++)
		{
			t += influence[i];
			if (t >= s)
			{
				return i;
				break;
			}
		}
		return 0;
	}

	void file_Dump(char *filename, IM *m)
	{
		FILE *f = fopen(filename, "w+");
		for (int i = 0; i < 256; i++) fprintf(f, "\t%3d", i);
		fprintf(f, "\n");
		for (int i = 0; i < 256; i++)
		{
			fprintf(f, "%3d", i);
			for (int j = 0; j < 256; j++)
			{
				fprintf(f, "\t%f", m->m[i][j]);
			}
			fprintf(f, "\n");
		}
		fclose(f);
	}

}

void t1()
{
	//char *filename = "test.txt";
	char *filename = "input.txt";
	char *output_filename = "log.txt";
	int window_size = 20;
	int gen_len = 10000;

	Random::init(0);
	unsigned char *buffer = NULL;
	unsigned int buffer_size = 0;

	printf("loading file\n");
	Util::load_File(buffer, buffer_size, filename);

	IM::IM m;
	IM::init(&m);

	printf("disassembling\n");
	IM::add_Disassembly(&m, buffer, buffer_size, window_size);

	printf("generating\n");
	double influence[256];
	unsigned char *tmp_buffer = new unsigned char[gen_len * 2];
	for (int i = 0; i < window_size; i++) tmp_buffer[i] = buffer[i];
	
	for (int pos = window_size; pos < gen_len; pos++)
	{
		IM::get_Influence_at_End(influence, &m, &tmp_buffer[pos - window_size], window_size);
		
		unsigned char c = IM::pick(influence);
		tmp_buffer[pos] = c;

#if DEBUG 1
		for (int i = 'a'; i <= 'z'; i++)
		{
			printf("%c -> %f", i, influence[i]);
			if (i == c)
			{
				printf(" <--- ");
			}
			printf("\n");
		}
		getchar();
#endif
	}
	tmp_buffer[gen_len] = 0;

	printf("writing file\n");
	FILE *f = fopen(output_filename, "w+");
	fwrite(tmp_buffer, 1, gen_len, f);
	fclose(f);
	
	printf("done\n");

	static char cmd[512];
	sprintf(cmd, "gvim %s", output_filename);
	system(cmd);
	
}

int main()
{
	t1();

	return 0;
}