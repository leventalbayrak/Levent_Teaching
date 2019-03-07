#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



namespace Utility
{
	void load_Block_of_Text(char *str, const char *filename)
	{
		//open the file
		FILE *f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		int filesize = ftell(f);
		fseek(f, 0, SEEK_SET);
		//dump the contents into str
		fread(str, 1, filesize, f);
		str[filesize] = 0;
		//close the file
		fclose(f);
	}

	void load_Conversation_Text(char **lines, int *number_of_lines, const char *filename)
	{
		//open the file
		FILE *f = fopen(filename, "r");
		*number_of_lines = 0;
		for (int i = 0; i < 10000; i++)
		{
			if (fgets(lines[i], 1000, f) == NULL) break;

			int length = strlen(lines[i]);
			if (lines[i][length - 1] == '\n') length--;
			lines[i][length] = 0;

			(*number_of_lines)++;
		}
		fclose(f);
	}

	void load_Integer_Grid(int **grid, int *number_of_rows, int *number_of_columns, const char* filename)
	{
		FILE *f = fopen(filename, "r");
		fscanf(f, "%d\t%d", number_of_rows, number_of_columns);

		for (int i = 0; i < *number_of_rows; i++)
		{
			for (int j = 0; j < *number_of_columns; j++)
			{
				fscanf(f, "%d", &grid[i][j]);
			}
		}

		fclose(f);
	}
}

namespace Game
{

	char *story = NULL;
	char *greeting = NULL;
	char **bear_conversation = NULL;
	int number_of_bear_conversation_lines = 0;
	char **food_lines = NULL;
	int number_of_food_lines = 0;
	
	int **map = NULL;
	int number_of_rows = 0;
	int number_of_columns = 0;

	int player_position_y = 1;
	int player_position_x = 1;

	void load_Stories()
	{
		story = (char *)malloc(sizeof(char) * 100000);
		Utility::load_Block_of_Text(story, "story.txt");

		greeting = (char *)malloc(sizeof(char) * 100000);
		Utility::load_Block_of_Text(greeting, "greeting.txt");
	}

	void load_Conversations()
	{
		bear_conversation = (char **)malloc(sizeof(char*) * 10000);
		for (int i = 0; i < 10000; i++)
		{
			bear_conversation[i] = (char *)malloc(sizeof(char) * 1000);
		}

		number_of_bear_conversation_lines = 0;
		Utility::load_Conversation_Text(bear_conversation, &number_of_bear_conversation_lines, "bear_conversation.txt");

		food_lines = (char **)malloc(sizeof(char*) * 10000);
		for (int i = 0; i < 10000; i++)
		{
			food_lines[i] = (char *)malloc(sizeof(char) * 1000);
		}
		number_of_food_lines = 0;
		Utility::load_Conversation_Text(food_lines, &number_of_food_lines, "food.txt");

	}

	void load_Maps()
	{
		map = (int**)malloc(sizeof(int*) * 1000);
		for (int i = 0; i < 1000; i++)
		{
			map[i] = (int*)malloc(sizeof(int) * 1000);
		}
		Utility::load_Integer_Grid(map, &number_of_rows, &number_of_columns, "grid.txt");
	}

	void say_Something()
	{
		printf("I GREET YOU TRAVELER\n");
		printf("---%s---\n", greeting);
		printf("here is a story:\n");
		printf("---%s---\n", story);
	}

	void bear_Request_Food()
	{
		printf("bear says:");
		for (int i = 0; i < 4; i++)
		{
			int which_line = rand() % number_of_bear_conversation_lines;
			printf("%s\n", bear_conversation[which_line]);
		}

		printf("bear wants a ");
		for (int i = 0; i < 3; i++)
		{
			int which_line = rand() % number_of_food_lines;
			printf(", %s", food_lines[which_line]);
		}
		printf(" sandwich!\n");
	}

	void move_Player()
	{
		printf("MOVE: ");
		char direction = 0;
		scanf("%c", &direction);
		getchar();

		if (direction == 'w')
		{
			if (map[player_position_y-1][player_position_x] == -1)
			{
				printf("you cant move there!\n");
			}
			else
			{
				player_position_y -= 1;
			}
		}
		else if (direction == 's')
		{
			if (map[player_position_y + 1][player_position_x] == -1)
			{
				printf("you cant move there!\n");
			}
			else
			{
				player_position_y += 1;
			}
		}
		else if (direction == 'a')
		{
			if (map[player_position_y][player_position_x-1] == -1)
			{
				printf("you cant move there!\n");
			}
			else
			{
				player_position_x -= 1;
			}
		}
		else if (direction == 'd')
		{
			if (map[player_position_y][player_position_x + 1] == -1)
			{
				printf("you cant move there!\n");
			}
			else
			{
				player_position_x += 1;
			}
		}
	}

	void print_Random_Room_Description()
	{
		printf("this room is empty\n");
	}

	void describe_Player_Room()
	{
		if (map[player_position_y][player_position_x] == -1)
		{
			printf("you should not be here!!!\n");
		}
		else if (map[player_position_y][player_position_x] == 0)
		{
			print_Random_Room_Description();
		}
		else if (map[player_position_y][player_position_x] == 1)
		{
			printf("you smell a foul odor coming from the south east corridor\n");
		}
		else if (map[player_position_y][player_position_x] == 2)
		{
			printf("you see a bear!!!!!\n");
			say_Something();
			bear_Request_Food();
		}
	}
}

int main()
{
	srand(time(0));

	Game::load_Stories();
	Game::load_Conversations();
	Game::load_Maps();
	
	for (;;)
	{
		Game::move_Player();
		Game::describe_Player_Room();
	}
	

	getchar();
	return 0;
}