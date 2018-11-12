#pragma warning(disable:4996)

#include "Word_Reader_core.h"

#include "String_core.h"
#include "String_Ext.h"

#include "Word_Stats_core.h"

int main()
{
	WR::Data my_last_words;
	WR::load_File(&my_last_words, "words.txt");

	int palindrome_count = 0;
	
	for (int i = 0; i < my_last_words.n_words; i++)
	{
		int r = Str_Ops::is_Palindrome(my_last_words.words[i]);
		if (r == 1)
		{
			palindrome_count++;
			printf("%d: %s\n", palindrome_count, my_last_words.words[i]);
		}
	}

	printf("number of palindromes: %d\n", palindrome_count);
	printf("number of words read: %d\n", my_last_words.n_words);

	double ratio = (double)palindrome_count / my_last_words.n_words;

	printf("ratio of palindromes: %f\n", ratio);

	//create a WS::Stats struct
	WS::Stats my_words_stats;
	//call calculate_Stats
	WS::calculate_Stats(&my_words_stats, (const char**)my_last_words.words, my_last_words.n_words);
	//call print_Stats
	WS::print_Stats(&my_words_stats);

	WR::Data filtered_words;
	WR::filter(&filtered_words, &my_last_words, 25, 50);

	WR::print(&filtered_words);

	getchar();
	return 0;
}