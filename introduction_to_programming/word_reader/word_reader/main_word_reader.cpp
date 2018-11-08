#pragma warning(disable:4996)

#include "Word_Reader_core.h"

#include "String_core.h"
#include "String_Ext.h"

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
	//call calculate_Stats
	//call print_Stats

	getchar();
	return 0;
}