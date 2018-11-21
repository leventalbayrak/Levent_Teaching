#pragma once

#include "Word_Reader_core.h"

namespace RF
{

	int is_Rhyming(const char* word, const char* rhyme_ending)
	{
		int word_length = Str_Ops::length(word);
		int rhyme_length = Str_Ops::length(rhyme_ending);
		
		for (int i = 0; i < rhyme_length; i++)
		{
			if (word[word_length - 1 - i] != rhyme_ending[rhyme_length - 1 - i])
			{
				return 0;
			}
		}

		return 1;
	}




	void print_Rhyming_Words(const WR::Data *input, const char* rhyme_with)
	{
		for (int i = 0; i < input->n_words; i++)
		{
			if (is_Rhyming(input->words[i],rhyme_with) == 1)
			{
				printf("%s\n", input->words[i]);
			}
		}
	}


}