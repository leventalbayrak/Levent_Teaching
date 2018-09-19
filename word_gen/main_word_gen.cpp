#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
namespace WG
{
	unsigned int n_alphabet = 256;

	void alloc(double *&p2D, double *&sum1D, unsigned int &n_nmers, unsigned int nmer_size)
	{
		n_nmers = n_Nmers(nmer_size);
		p2D = (double*)malloc(sizeof(double)*n_nmers*n_alphabet); assert(p2D);
		sum1D = (double*)malloc(sizeof(double)*n_nmers); assert(sum1D);
	}

	inline unsigned int n_Nmers(unsigned int nmer_size)
	{
		return 1 << (nmer_size << 3);
	}

	unsigned int get_Nmer_Profile_Index(const char *nmer, unsigned int nmer_size)
	{
		unsigned int s = 0;
		unsigned int m = 1;
		for (unsigned int i = 0; i < nmer_size; i++)
		{
			s += nmer[i] * m;
			m <<= 8;
		}	
		return s*n_alphabet;
	}
	void extract_Counts(double *p2D,double *sum1D, const char **words, unsigned int n_words, unsigned int nmer_size)
	{
		int extract_size = nmer_size + 1;
		for (int i = 0; i < n_words; i++)
		{
			int len = strlen(words[i]);
			for (int j = 0; j <= len - extract_size; j++)
			{
				unsigned int nmer_index = get_Nmer_Profile_Index(&words[i][j], nmer_size);
				unsigned int offset = words[i][j + nmer_size];
				p2D[nmer_index + offset] += 1.0;
				sum1D[nmer_index] += 1.0;
			}
		}
	}

	void normalize(double *p, double *sum, unsigned int nmer_size)
	{
		unsigned int n_nmers = n_Nmers(nmer_size);
		for (unsigned int i = 0; i < n_nmers; i++)
		{
			for(unsigned int j=0;j<n_alphabet;j++) p[i*n_alphabet + j] /= sum[i];
		}
	}

}

int main()
{
	unsigned int nmer_size = 3;

	double *profiles = NULL;
	double *sum = NULL;
	unsigned int n_nmers = 0;
	WG::alloc(profiles, sum, n_nmers, nmer_size);

	char *words[4] = {"levent","rita","kamil","otto"};
	int n_words = 4;

	WG::extract_Counts(profiles, sum, words, n_words, nmer_size);



	return 0;
}