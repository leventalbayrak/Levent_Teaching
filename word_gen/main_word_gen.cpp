#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
namespace WG
{
	unsigned int get_Index(const char *nmer, int nmer_size)
	{
		unsigned int s = 0;
		unsigned int m = 1;
		for (unsigned int i = 0; i < nmer_size; i++)
		{
			s += nmer[i] * m;
			m *= 256;
		}	
		return s;
	}

	void generate(double **p,double *sum, const char **words, int n_words, int nmer_size)
	{
		int extract_size = nmer_size + 1;
		for (int i = 0; i < n_words; i++)
		{
			int len = strlen(words[i]);
			for (int j = 0; j <= len - extract_size; j++)
			{
				unsigned int nmer_index = get_Index(&words[i][j], nmer_size);
				p[nmer_index][words[i][j + nmer_size]] += 1.0;
				sum[nmer_index] += 1.0;
			}
		}
	}

}

int main()
{




	return 0;
}