#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)



int main()
{
	srand(time(0));

	const int number_of_tiles = 60;
	static int tiles[number_of_tiles];

	const char* tile_type_string[4] = { 
		"grass",
		"slow trap" ,
		"stop trap",
		"obstacle" 
	};

	const float tile_probability[4] =
	{
		0.9,//90% grass
		0.02,//2% slow
		0.02,//2% stop
		0.06//6% obstacle
	};

	for (int i = 0; i < number_of_tiles; ++i)
	{
		float roll = (float)rand() / RAND_MAX;

		float sum = 0.0f;
		for (int tile_type = 0; tile_type < 4; ++tile_type)
		{
			sum += tile_probability[tile_type];
			if (roll <= sum)
			{
				tiles[i] = tile_type;
				break;
			}
		}
	}
	/*
	print tiles to a file for debugging
	*/
	FILE* f = fopen("log.txt", "w");
	for (int i = 0; i < number_of_tiles; ++i)
	{
		fprintf(f, "%d\n", tiles[i]);
	}
	fclose(f);

	int bear_position = 0;
	int bear_speed = 2;

	int player_position = 10;
	int player_speed = 1;

	for (;;)
	{

		int distance_to_bear = player_position - bear_position;
		printf("the bear is %d meters away from you!\n", distance_to_bear);

		int player_tile = tiles[player_position];
		printf("you are standing on %s\n", tile_type_string[player_tile]);

		printf("what would you like to do?\n");

		if (player_tile == 0)
		{
			printf("(s)hoot\n");
			printf("(r)un\n");
		}
		else if (player_tile == 1 || player_tile == 2)
		{
			printf("(s)hoot\n");
			printf("(r)un\n");
			printf("(t)rap\n");
		}
		else if (player_tile == 3)
		{
			printf("(s)hoot\n");
			printf("(c)lear obstacle\n");
		}

		char command = getchar();
		getchar();
		printf("you chose %c\n", command);

		if (command == 'r')
		{
			player_position += player_speed;
		}
		else if (command == 'c')
		{
			tiles[player_position] = 0;
		}
		else if (command == 's')
		{
			float roll = (float)rand() / RAND_MAX;
			if (roll <= 0.3)
			{
				printf("you missed!\n");
				bear_speed += 1;
			}
			else
			{
				printf("you hit the bear!\n");
				bear_speed -= 1;
			}
		}

		for (int i = 0; i < bear_speed; ++i)
		{
			bear_position += 1;
			int bear_tile = tiles[bear_position];
			printf("the bear entered %s\n", tile_type_string[bear_tile]);
		}

		for (int i = 0; i < number_of_tiles; ++i)
		{
			if (i == player_position)
			{
				printf("P");
			}
			else if (i == bear_position)
			{
				printf("B");
			}
			else
			{
				printf("-");
			}
		}
		printf("\n");

	}


	return 0;
}