#pragma once

#include "Word_Stats_data.h"
#include "String_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace WS
{

	void calculate_Stats(Stats *stats, const char **words, int n_words)
	{
		int *word_lengths = (int*)malloc(sizeof(int)*n_words);
		
		for (int i = 0; i < n_words; i++)
		{
			word_lengths[i] = Str_Ops::length(words[i]);
		}

		stats->avg = 0.0;
		stats->stdev = 0.0;
		stats->n_words = n_words;
		
		double sum = 0.0;
		for (int i = 0; i < n_words; i++)
		{
			sum += word_lengths[i];
		}
		stats->avg = sum / n_words;

		double sum_squared = 0.0;
		for (int i = 0; i < n_words; i++)
		{
			double value = (word_lengths[i] - stats->avg);
			sum_squared += value * value;
		}
		stats->stdev = sqrt(sum_squared / n_words);

		stats->min = word_lengths[0];
		stats->max = word_lengths[0];
		for (int i = 1; i < n_words; i++)
		{
			if (word_lengths[i] < stats->min)
			{
				stats->min = word_lengths[i];
			}

			if (word_lengths[i] > stats->max)
			{
				stats->max = word_lengths[i];
			}
		}

		free(word_lengths);
	}

	void print_Stats(const Stats *stats)
	{
		//print all data members
		printf("min = %d\nmax = %d\navg = %f\nstdev = %f\nN = %d\n",
			stats->min,
			stats->max,
			stats->avg,
			stats->stdev,
			stats->n_words
		);
	}

}