#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable:4996)


int main()
{
	srand(time(0));

	const int number_of_tiles = 60;
	static int tiles[number_of_tiles];

	const char* tile_type_string[2] = { 
		"grass",
		"forest",
	};

	const float tile_probability[2] =
	{
		0.8,//80% grass
		0.2,//20% forest
	};

	for (int i = 0; i < number_of_tiles; ++i)
	{
		float roll = (float)rand() / RAND_MAX;

		float sum = 0.0f;
		for (int tile_type = 0; tile_type < 2; ++tile_type)
		{
			sum += tile_probability[tile_type];
			if (roll <= sum)
			{
				tiles[i] = tile_type;
				break;
			}
		}
	}

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

		int bear_tile = tiles[bear_position];
		printf("the bear is standing on %s\n", tile_type_string[bear_tile]);

		printf("what would you like to do?\n");

		printf("(s)hoot\n");
		printf("(r)un\n");

		char command = getchar();
		getchar();
		
		printf("you chose %c\n", command);

		if (command == 'r')
		{
			player_position += player_speed;
		}
		else if (command == 's')
		{
			float roll = (float)rand() / RAND_MAX;

			float miss_chance = 0.3f;
			if (bear_tile == 1)//forest
			{
				miss_chance = 0.6f;
			}

			if (roll <= miss_chance)
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

		if (bear_position >= player_position)
		{
			printf("you are dead!\n");
			break;
		}
		if (bear_speed <= 0)
		{
			printf("bear stopped chasing you!\n");
			break;
		}
		if (player_position == number_of_tiles - 1)
		{
			printf("you made it home!\n");
			break;
		}	
	}

	getchar();
	return 0;
}