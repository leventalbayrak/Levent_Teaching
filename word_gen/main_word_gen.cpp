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
	unsigned char alphabet[256];
	int idx[256];
	unsigned int n_alphabet = 0;

	

	namespace Random
	{
		mt19937 engine;
		static void init(unsigned int _seed)
		{
			if (_seed == 0) _seed = time(0);
			engine.seed(_seed);
		}
		inline unsigned int rand_UINT() { return engine(); }
		inline double rand_DOUBLE() { return (double)engine() / engine.max(); }
	}

	inline unsigned int n_Nmers(unsigned int nmer_size)
	{
		return pow(n_alphabet,nmer_size);
	}

	void init()
	{
		Random::init(0);

		alphabet[0] = 0;
		n_alphabet = 1;
		for (int i = 33; i <= 126; i++)
		{
			alphabet[n_alphabet++] = i;
		}

		idx[0] = 0;
		int k = 1;
		for (int c = 33; c <= 126; c++)
		{
			idx[c] = k++;
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

	void alloc(double *&p2D, double *&sum1D, unsigned int &n_nmers, unsigned int nmer_size)
	{
		n_nmers = n_Nmers(nmer_size);
		printf("n_nmers: %u allocation: %.2fgb\n", n_nmers, sizeof(double)*n_nmers*(n_alphabet + 1) / (1024.0*1024.0*1024.0));
		p2D = (double*)malloc(sizeof(double)*n_nmers*n_alphabet); assert(p2D);
		sum1D = (double*)malloc(sizeof(double)*n_nmers); assert(sum1D);

		memset(p2D, 0, sizeof(double)*n_nmers*n_alphabet);
		memset(sum1D, 0, sizeof(double)*n_nmers);
	}

	unsigned int nmer_to_uint(const char *nmer, unsigned int nmer_size)
	{
		unsigned int s = 0;
		unsigned int m = 1;
		for (unsigned int i = 0; i < nmer_size; i++)
		{
			s += idx[nmer[i]] * m;
			m *= n_alphabet;
		}
		return s;
	}

	void uint_to_nmer(char *nmer, unsigned int nmer_uint, unsigned int nmer_size)
	{
		unsigned int s = nmer_uint;
		for (unsigned int i = 0; i < nmer_size; i++)
		{
			nmer[i] =  alphabet[s % n_alphabet];
			s /= n_alphabet;
		}
		nmer[nmer_size] = 0;
	}

	unsigned int get_Nmer_Profile_Index_1D(const char *nmer, unsigned int nmer_size)
	{
		return nmer_to_uint(nmer, nmer_size) * n_alphabet;
	}

	void extract_Counts(double *p2D, double *sum1D, const char **words, unsigned int n_words, unsigned int nmer_size)
	{
		for (int i = 0; i < n_words; i++)
		{
			int len = strlen(words[i]);
			if (len < nmer_size) continue;

			if (len != nmer_size)
			{
				for (unsigned int j = 0; j <= len - nmer_size - 1; j++)
				{
					unsigned int nmer_index = get_Nmer_Profile_Index_1D(&words[i][j], nmer_size);
					unsigned int offset = idx[words[i][j + nmer_size]];
					p2D[nmer_index + offset] += 1.0;
					sum1D[nmer_index / n_alphabet] += 1.0;
				}
			}
			//zero terminator
			unsigned int nmer_index = get_Nmer_Profile_Index_1D(&words[i][len - nmer_size], nmer_size);
			p2D[nmer_index] += 1.0;
			sum1D[nmer_index / n_alphabet] += 1.0;
		}
	}

	//will also normalize sum
	void normalize(double *normalized_profile, double *normalized_sum, unsigned int nmer_size)
	{
		unsigned int n_nmers = n_Nmers(nmer_size);

		double total_sum = 0.0;
		for (unsigned int i = 0; i < n_nmers; i++)
		{
			if (normalized_sum[i] != 0)
			{
				for (unsigned int j = 0; j < n_alphabet; j++) normalized_profile[i*n_alphabet + j] /= normalized_sum[i];
				total_sum += normalized_sum[i];
			}
		}

		for (unsigned int i = 0; i < n_nmers; i++)
		{
			normalized_sum[i] /= total_sum;
		}

	}

	void gen_Seed(char *seed,const double *normalized_sum, unsigned int nmer_size)
	{
		unsigned int n_nmers = n_Nmers(nmer_size);
		double s = 0.0;
		double c = Random::rand_DOUBLE();
		unsigned int k = 0;
		for (unsigned int i = 0; i < n_nmers; i++)
		{
			s += normalized_sum[i];
			if (s >= c)
			{
				k = i;
				break;
			}
		}

		uint_to_nmer(seed, k, nmer_size);
	}

	void gen_Word(char *dest, const char *seed, const double *p2D, unsigned int nmer_size)
	{
		for (unsigned int i = 0; i < nmer_size; i++) dest[i] = seed[i];

		unsigned int pos = 0;
		for (;;)
		{
			unsigned int nmer_index = get_Nmer_Profile_Index_1D(&dest[pos], nmer_size);
			double c = Random::rand_DOUBLE();
			double s = 0.0;
			int k = 0;
			for (int i = 0; i < n_alphabet; i++)
			{
				s += p2D[nmer_index + i];
				if (s >= c)
				{
					k = i;
					break;
				}
			}
			dest[pos + nmer_size] = alphabet[k];
			pos++;
			if (k == 0) break;
		}

	}
}

int main()
{
	unsigned int nmer_size = 3;
	unsigned int n_gen = 20000;
	char *filename_dictionary = (char*)"words_alpha.txt";
	char *filename_gen_output = (char*)"generated_words.txt";

	char **words = NULL;
	unsigned int n_words = 0;
	WG::init();
	WG::load_Dictionary(words, n_words, filename_dictionary);

	printf("loaded %u words\n", n_words);

	

	double *profiles = NULL;
	double *sum = NULL;
	unsigned int n_nmers = 0;
	WG::alloc(profiles, sum, n_nmers, nmer_size);
	//getchar();
	WG::extract_Counts(profiles, sum, (const char**)words, n_words, nmer_size);
	WG::normalize(profiles, sum, nmer_size);

	unsigned int n_total_nmers = 0;
	for (unsigned int i = 0; i < n_nmers; i++) n_total_nmers += sum[i];

	FILE *f = fopen(filename_gen_output, "w+");
	static char seed[1024];
	static char dest[1024];
	unsigned int completed_counter = 0;
	for (unsigned int i = 0; i < n_gen; i++)
	{
		if (++completed_counter > n_gen / 100)
		{
			printf("completed %.2f\n", 100.0*i / n_gen);
			completed_counter = 0;
		}
		WG::gen_Seed(seed, sum, nmer_size);
		WG::gen_Word(dest, seed, profiles, nmer_size);
		fprintf(f, "%s\n", dest);

		if (strcmp("wolf", dest) == 0)
		{
			printf("found wolf\n");
			getchar();
		}
	}
	fclose(f);
	return 0;
}