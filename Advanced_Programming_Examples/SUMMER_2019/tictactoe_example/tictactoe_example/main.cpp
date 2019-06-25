#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//3x3 grid

void draw(int grid[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		printf("|");
		for (int j = 0; j < 3; j++)
		{
			printf("%d|", grid[i][j]);
		}
		printf("\n");
	}
}

int put(int grid[3][3], int value, int row, int col)
{
	if (grid[row][col] != 0) return 0;
	grid[row][col] = value;
	return 1;
}

int win(int grid[3][3])
{
	//check rows
	for (int i = 0; i < 3; i++)
	{
		if (grid[i][0] != 0 &&
			grid[i][0] == grid[i][1] &&
			grid[i][0] == grid[i][2])
		{
			return grid[i][0];
		}
	}

	//check columns
	for (int i = 0; i < 3; i++)
	{
		if (grid[0][i] != 0 &&
			grid[0][i] == grid[1][i] &&
			grid[0][i] == grid[2][i])
		{
			return grid[0][i];
		}
	}

	if (grid[0][0] != 0 &&
		grid[0][0] == grid[1][1] &&
		grid[0][0] == grid[2][2])
	{
		return grid[0][0];
	}

	if (grid[0][2] != 0 &&
		grid[0][2] == grid[1][1] &&
		grid[0][2] == grid[2][0])
	{
		return grid[0][2];
	}

	return 0;
}

void player_Input(int grid[3][3], int player_id)
{
	for (;;)
	{
		printf("player %d row col:", player_id);
		int row = 0;
		int col = 0;
		scanf("%d %d", &row, &col);

		int put_success = put(grid, player_id, row, col);
		if (put_success == 0)
		{
			printf("%d %d is occupied, try again\n", row, col);
		}
		else
		{
			break;
		}
	}
}

int main()
{
	int grid[3][3] = {
		{0,0,0},
		{0,0,0},
		{0,0,0},
	};

	int turn_counter = 0;
	for (;;)
	{
		if (turn_counter == 0)
		{
			printf("resetting board\n");
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					grid[i][j] = 0;
				}
			}
		}

		turn_counter++;

		if (turn_counter == 10)
		{
			turn_counter = 0;
			printf("no winners!\n");
			continue;
		}

		draw(grid);

		if (turn_counter % 2 == 0)
		{
			player_Input(grid, 2);
		}
		else
		{
			player_Input(grid, 1);
		}

		int winner = win(grid);
		if (winner != 0)
		{
			turn_counter = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					grid[i][j] = 0;
				}
			}
			printf("player %d won!", winner);

		}

	}

	getchar();
	return 0;
}

