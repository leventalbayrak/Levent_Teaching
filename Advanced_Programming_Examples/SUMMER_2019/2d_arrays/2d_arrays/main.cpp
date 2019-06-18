#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
/*
description
monopoly
*/

namespace Board
{

	int** create(int nrows, int ncols)
	{
		int **board = (int**)malloc(sizeof(int*)*nrows);

		for (int i = 0; i < nrows; i++)
		{
			board[i] = (int*)malloc(sizeof(int)*ncols);

			for (int j = 0; j < ncols; j++) board[i][j] = -1;
		}

		return board;
	}

	void set(int **board, int x, int y, int value)
	{
		board[y][x] = value;
	}

	int get(int **board, int x, int y)
	{
		return board[y][x];
	}

	void draw(int **board, int nrows, int ncols)
	{
		for (int i = 0; i < nrows; i++)
		{
			printf("|");
			for (int j = 0; j < ncols; j++)
			{
				if (board[i][j] == -1)
				{
					printf(" |");
				}
				else
				{
					printf("%d|", board[i][j]);
				}
			}
			printf("\n");
		}
	}
}

int main()
{
	int nrows = 6;
	int ncols = 6;
	int **board = Board::create(nrows, ncols);
	
	int number_of_players = 4;
	float *balance = (float*)malloc(sizeof(float)*number_of_players);
	for (int i = 0; i < number_of_players; i++)
	{
		balance[i] = 1000.0;
	}
	
	for (;;)
	{
		int last_x = 0;
		int last_y = 0;
		printf("monopoly>");
		char command[10];
		scanf("%s", command);
		if (strcmp(command, "check") == 0)
		{
			int player = 0;
			scanf("%d", &player);
			if (player < number_of_players)
			{
				printf("player %d balance is %.2f\n", player, balance[player]);
			}
			else
			{
				printf("player does not exist\n");
			}
		}
		else if (strcmp(command, "buy") == 0)
		{
			int x = 0;
			int y = 0;
			int player = 0;
			float amount = 0.0;
			scanf("%d", &y);
			scanf("%d", &x);
			scanf("%d", &player);


			if (x >= ncols || y >= nrows || x < 0 || y < 0) 
			{
				printf("This space does not exist\n");
				continue;
			}

			last_x = x;
			last_y = y;


			int seller = board[y][x];
			if (seller == -1)
			{
				amount = 200.0;
				balance[player] -= amount;
				Board::set(board, x, y, player);
				
			}
			else
			{
				printf("spot is owned by %d\n", seller);
				printf("would seller sell?");
				char answer[5];
				scanf("%s", answer);
				if (answer[0] == 'y')
				{
					scanf("%f", &amount);
					balance[player] -= amount;
					balance[seller] += amount;
					Board::set(board, x, y, player);
				}
				else
				{
					float fee = 25;
					balance[player] -= fee;
				}
			}

		}

		for (int i = 0; i < nrows; i++)
		{
			for (int j = 1; j < ncols-1; j++)
			{
				if (i == last_y && j == last_x)
				{
					continue;
				}
				int left = board[i][j - 1];
				int right = board[i][j + 1];
				
				if (left == right )
				{
					if (left != -1)
					{
						board[i][j] = left;
					}			
				}
			}
		}

		for (int i = 1; i < nrows-1; i++)
		{
			for (int j = 0; j < ncols; j++)
			{
				if (i == last_y && j == last_x)
				{
					continue;
				}
				int up = board[i-1][j];
				int down = board[i+1][j];

				if (up == down)
				{
					if (up != -1)
					{
						board[i][j] = up;
					}
				}
			}
		}

		Board::draw(board, nrows, ncols);
		for (int i = 0; i < number_of_players; i++)
		{
			printf("player %d = %.2f\n", i, balance[i]);
		}
	}



	getchar();

	return 0;
}