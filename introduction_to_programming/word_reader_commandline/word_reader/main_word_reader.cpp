#pragma warning(disable:4996)
#include <time.h>
#include <string.h>
#include "Word_Reader_core.h"

#include "String_core.h"
#include "String_Ext.h"

#include "Word_Stats_core.h"

#include "Rhyme_Finder_core.h"

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
		else if (strcmp(command, "rhyme") == 0)
		{
			if (r == 2)
			{
				RF::print_Rhyming_Words(&words, parameter);
			}
			else
			{
				printf("ERROR: enter a rhyme ending\n");
			}

		}
		else if (strcmp(command, "length") == 0)
		{
			if (r == 2)
			{
				int length_parameter = atoi(parameter);
				for (int i = 0; i < words.n_words; i++)
				{
					int word_length = Str_Ops::length(words.words[i]);

					if (word_length == length_parameter)
					{
						printf("%s\n", words.words[i]);
					}
				}
			}
			else
			{
				printf("ERROR: enter a length\n");
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

	return 0;
}