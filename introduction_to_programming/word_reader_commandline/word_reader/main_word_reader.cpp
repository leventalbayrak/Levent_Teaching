#pragma warning(disable:4996)
#include <time.h>
#include <string.h>
#include "Word_Reader_core.h"

#include "String_core.h"
#include "String_Ext.h"

#include "Word_Stats_core.h"

int my_Getline(char *str)
{
	int length = 0;

	for (;;)
	{
		int r = getchar();

		if (r == '\n' || r == '\r')
		{
			break;
		}
		else
		{
			str[length++] = r;
		}
	}

	str[length] = 0;

	return length;
}

int main()
{
	srand(time(0));

	WR::Data words = {};
	WS::Stats words_stats;
	int stats_have_been_calculated = 0;
	
	for (;;)
	{
		char line[256];
		char command[256];
		char parameter[256];

		printf("NGFE>");

		int line_length = my_Getline(line);
		int r = sscanf(line, "%s %s", command, parameter);

		if (strcmp(command, "load") == 0)
		{
			if (r == 2)
			{
				if (words.n_words != 0)
				{
					printf("unloading...\n");
					WR::destroy(&words);
					printf("done\n");
				}
				printf("loading %s...\n", parameter);
				WR::load_File(&words, parameter);
				printf("done\n");
			}
			else
			{
				printf("ERROR: enter a filename\n");
			}
		}
		else if (strcmp(command, "stats") == 0)
		{
			if (words.n_words == 0)
			{
				printf("please use:\n'load filename.txt'\n");
			}
			else
			{
				if (stats_have_been_calculated == 0)
				{
					printf("calculating stats...\n", parameter);
					WS::calculate_Stats(&words_stats, (const char**)words.words, words.n_words);
					stats_have_been_calculated = 1;
					printf("done\n");
				}
				WS::print_Stats(&words_stats);
			}
			
		}
		else if (strcmp(command, "print") == 0)
		{
			if (words.n_words == 0)
			{
				printf("please use:\n'load filename.txt'\n");
			}
			else
			{
				WR::print(&words);
			}	
		}
		else if (strcmp(command, "less") == 0)
		{
			if (words.n_words == 0)
			{
				printf("please use:\n'load filename.txt'\n");
			}
			else
			{
				WR::less(&words, 10);
			}
		}
		else if (strcmp(command, "tail") == 0)
		{
			if (words.n_words == 0)
			{
				printf("please use:\n'load filename.txt'\n");
			}
			else
			{
				WR::tail(&words, 10);
			}
		}
		else if (strcmp(command, "random") == 0)
		{
			if (words.n_words == 0)
			{
				printf("please use:\n'load filename.txt'\n");
			}
			else
			{
				WR::random(&words, 10);
			}
		}
		else
		{
			printf("unknown command\n");
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	system("pause");

	//for (;;)
	//{
	//	printf(">");

	//	int r_command = scanf("%s", command);

	//	printf("you entered %s\n", command);

	//	int r_parameter = scanf("%s", parameter);

	//	if (r_command == 1)
	//	{
	//		if (r_parameter == 1)
	//		{

	//			printf("your command has parameter: %s\n", parameter);
	//		}
	//		else
	//		{
	//			printf("your command does not have a parameter\n");
	//		}
	//		
	//	}
	//	else 
	//	{
	//		printf("unknown command\n");
	//	}
	//}










	//WR::Data my_last_words;
	//WR::load_File(&my_last_words, "words.txt");

	//int palindrome_count = 0;
	//
	//for (int i = 0; i < my_last_words.n_words; i++)
	//{
	//	int r = Str_Ops::is_Palindrome(my_last_words.words[i]);
	//	if (r == 1)
	//	{
	//		palindrome_count++;
	//		printf("%d: %s\n", palindrome_count, my_last_words.words[i]);
	//	}
	//}

	//printf("number of palindromes: %d\n", palindrome_count);
	//printf("number of words read: %d\n", my_last_words.n_words);

	//double ratio = (double)palindrome_count / my_last_words.n_words;

	//printf("ratio of palindromes: %f\n", ratio);

	////create a WS::Stats struct
	//WS::Stats my_words_stats;
	////call calculate_Stats
	//WS::calculate_Stats(&my_words_stats, (const char**)my_last_words.words, my_last_words.n_words);
	////call print_Stats
	//WS::print_Stats(&my_words_stats);

	//WR::Data filtered_words;
	//WR::filter(&filtered_words, &my_last_words, 25, 50);

	//WR::print(&filtered_words);

	//getchar();
	return 0;
}