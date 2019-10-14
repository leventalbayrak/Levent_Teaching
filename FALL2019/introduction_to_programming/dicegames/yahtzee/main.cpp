#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)

int main()
{
	srand(time(0));

	printf("\
		.------ - .______\n\
		/ o   /|   /\     \\\n\
		/ _______ / o|  /o \  o  \\\n\
		| o     | | /   o\_____\\\n\
		| o | o / \o / o    /\\\n\
		|     o |/   \ o / o /\\\n\
		'-------'     \ / ____o /	\n\
		");
	printf("YAHTZEE??\n");

	for (;;)
	{
		printf("\n***NEXT PLAYER: press any key to roll***\n");
		getchar();
		
		int dice[5] = { 0 };

		printf("rolling...\n");
		for (int i = 0; i < 5; ++i) dice[i] = 1 + rand() % 6;

		printf("you rolled\n");
		for (int i = 0; i < 5; ++i)
		{
			printf("%d ", dice[i]);
		}
		printf("\n");

		for (int reroll = 0; reroll < 2; ++reroll)
		{
			printf("reroll round %d\n", reroll + 1);
			printf("enter 1 to hold 0 to discard\n");
			char hold[6];
			scanf("%s", hold);
			for (int i = 0; i < 5; ++i)
			{
				if (hold[i] == '0')
				{
					dice[i] = 1 + rand() % 6;
				}
			}

			printf("new roll\n");
			for (int i = 0; i < 5; ++i)
			{
				printf("%d ", dice[i]);
			}
			printf("\n");
		}

		printf("your final hand:\n");
		for (int i = 0; i < 5; ++i)
		{
			printf("%d ", dice[i]);
		}
		printf("\n");

		getchar();
	}

	getchar();
	return 0;
}