#include <stdio.h>


int main()
{
	static int dungeon[5][6] = { 0 };//fills with zeros, only possible at creation

	/*
	1 1 1 1 1 1
	1 0 0 0 0 1
	1 1 3 1 0 1
	1 2 0 0 0 1
	1 1 1 1 1 1 
	*/

	//fill first row
	for (int i = 0; i < 6; ++i) dungeon[0][i] = 1;
	//second row
	dungeon[1][0] = 1;
	dungeon[1][5] = 1;
	//third row
	dungeon[2][0] = 1;
	dungeon[2][1] = 1;
	
	dungeon[2][2] = 3;//death trap
	
	dungeon[2][3] = 1;
	dungeon[2][5] = 1;
	//fourth row
	dungeon[3][0] = 1;
	dungeon[3][1] = 2;
	dungeon[3][5] = 1;
	//last row
	for (int i = 0; i < 6; ++i) dungeon[4][i] = 1;

	int player_row = 1;
	int player_col = 1;

	for (;;)
	{
		printf("enter wasd:");
		char dir = getchar();
		getchar();

		if (dir == 'w')
		{
			if (dungeon[player_row - 1][player_col] == 1)
			{
				printf("there is a wall here, you cannot move!\n");
			}
			else
			{
				player_row--;
				printf("you move north...\n");
			}
		}
		if (dir == 's')
		{
			if (dungeon[player_row + 1][player_col] == 1)
			{
				printf("there is a wall here, you cannot move!\n");
			}
			else
			{
				player_row++;
				printf("you move south...\n");
			}
		}
		if (dir == 'a')
		{
			if (dungeon[player_row][player_col - 1] == 1)
			{
				printf("there is a wall here, you cannot move!\n");
			}
			else
			{
				player_col--;
				printf("you move west...\n");
			}
		}
		if (dir == 'd')
		{
			if (dungeon[player_row][player_col + 1] == 1)
			{
				printf("there is a wall here, you cannot move!\n");
			}
			else
			{
				player_col++;
				printf("you move east...\n");
			}
		}

		//check treasure
		if (dungeon[player_row][player_col] == 2)
		{
			printf("you found the treasure!\n");
			break;
		}
		if (dungeon[player_row][player_col] == 3)
		{
			printf("you die (fell into a hole)!\n");
			break;
		}
	}

	getchar();
	return 0;
}