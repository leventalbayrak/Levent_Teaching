#pragma once

#include "Word_Stats_data.h"
#include "String_core.h"
#include <stdio.h>
#include <stdlib.h>

namespace WS
{

	void calculate_Stats(Stats *stats, const char **words, int n_words)
	{
		int *word_lengths = (int*)malloc(sizeof(int)*n_words);
		
		/*
		do your calculations here
		*/


		free(word_lengths);
	}

	void print_Stats(const Stats *stats)
	{
		//print all data members
	}

}