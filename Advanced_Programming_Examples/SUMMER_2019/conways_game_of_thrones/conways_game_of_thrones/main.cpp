/*
just in case you do not understand the rules: https://www.youtube.com/watch?v=FWSR_7kZuYg
http://www.conwaylife.com/wiki/Conway%27s_Game_of_Life
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

SDL_Renderer *renderer = NULL;
int screen_w = 1280;
int screen_h = 800;

/*
10 pts if everything compiles and correctly works
*/

/*
10 pts
*/
void copy_Grid(int **dest, int **src, int nrows, int ncols)
{
	/*
	copy contents of source into destination, 1 element at a time
	*/
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

/*
80 pts
*/
void update(int **grid, int **scratch_grid, int nrows, int ncols)
{
	/*
	go over each cell in grid
	do conway's game of life logic (check 8 neighbors)
	note: the grid must WRAP AROUND (played on a donut)
	write the result into scratch grid
	*/

	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			//this is a dead cell
			if (grid[i][j] == 0)
			{
				int neighbor_list[8] = { 0 };
				int number_of_alive_neighbors = 0;
				//count and record neighbors (place neighbors in a list)
				for (int a = -1; a <= 1; a++)
				{
					for (int b = -1; b <= 1; b++)
					{
						if (grid[(i + a + nrows)%nrows][(j + b + ncols) % ncols] != 0)
						{
							neighbor_list[number_of_alive_neighbors++] = grid[(i + a + nrows) % nrows][(j + b + ncols) % ncols];
						}
					}
				}

				//if this dead cell has exactly 3 neighbors
				//and they are all the same type (species), then make dead cell alive
				if (number_of_alive_neighbors == 3)
				{
					if (neighbor_list[0] == neighbor_list[1] && neighbor_list[0] == neighbor_list[2])
					{
						scratch_grid[i][j] = neighbor_list[0];
					}
				}
				//done processing dead cell
				continue;
			}

			//this is a live cell
			int number_of_neighbors = 0;
			for (int a = -1; a <= 1; a++)
			{
				for (int b = -1; b <= 1; b++)
				{
					if (grid[i][j] == grid[(i + a + nrows) % nrows][(j + b + ncols) % ncols])
					{
						number_of_neighbors++;
					}
				}
			}

			number_of_neighbors -= 1;//because you counted yourself at i,j

			if (number_of_neighbors < 2)
			{
				scratch_grid[i][j] = 0;
			}
			else if (number_of_neighbors > 3)
			{
				scratch_grid[i][j] = 0;
			}
			else
			{
				scratch_grid[i][j] = grid[i][j];
			}
		}
	}


	/*
	once above task is completed,
	copy scratch grid into grid
	*/
	copy_Grid(grid, scratch_grid, nrows, ncols);
}

void randomize_Grid(int **grid, int nrows, int ncols)
{
	for (int i = 0; i < nrows/2; i++)
	{
		for (int j = 0; j < ncols/2; j++)
		{
			grid[i][j] = 1;
		}
	}
	for (int i = 0; i < nrows / 2; i++)
	{
		for (int j = ncols / 2; j < ncols; j++)
		{
			grid[i][j] = 2;
		}
	}
	for (int i = nrows / 2; i < nrows; i++)
	{
		for (int j = 0; j < ncols / 2; j++)
		{
			grid[i][j] = 3;
		}
	}
	for (int i = nrows / 2; i < nrows; i++)
	{
		for (int j = ncols / 2; j < ncols; j++)
		{
			grid[i][j] = 4;
		}
	}

	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			float p = (float)rand() / RAND_MAX;
			if (p < 0.4) grid[i][j] = 0;
		}
	}
}

void draw_Grid(int **grid, int nrows, int ncols, int width, int height)
{
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			int grid_value = grid[i][j];
			int cell_w = width / ncols;
			int cell_h = height / nrows;
			int cell_y = i * cell_h;
			int cell_x = j * cell_w;

			int r = 0;
			int g = 0;
			int b = 0;
			if(grid_value == 1)
			{
				r = 255;
				g = 0;
				b = 0;
			}	
			else if (grid_value == 2)
			{
				r = 58;
				g = 155;
				b = 20;
			}
			else if (grid_value == 3)
			{
				r = 0;
				g = 119;
				b = 190;
			}
			else if (grid_value == 4)
			{
				r = 240;
				g = 255;
				b = 0;
			}
			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		
			SDL_Rect rect = { cell_x,cell_y,cell_w,cell_h };
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("conway's game of life", 50, 50, screen_w, screen_h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(0));

	//allocate grid
	int nrows = screen_h / 10;
	int ncols = screen_w / 10;
	int **grid = (int **)malloc(sizeof(int*)*nrows);
	int **scratch_grid = (int **)malloc(sizeof(int*)*nrows);
	for (int i = 0; i < nrows; i++)
	{
		grid[i] = (int *)malloc(sizeof(int)*ncols);
		scratch_grid[i] = (int *)malloc(sizeof(int)*ncols);
	}

	//randomize grid
	randomize_Grid(grid, nrows, ncols);

	unsigned int last_frame_time = SDL_GetTicks();
	unsigned int last_sprinkle_time = last_frame_time;
	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					for (int i = 0; i < 0.2*nrows*ncols; i++)
					{
						int row = rand() % nrows;
						int col = rand() % ncols;
						grid[row][col] = 1 + rand() % 4;
					}
				}
			}
		}

		
		//frames per second (FPS) is:
		//fps = 1000/frame_time_in_milliseconds
		//limits frame time to 15 milliseconds
		unsigned int current_time = SDL_GetTicks();
		if (current_time - last_frame_time < 15)
		{
			continue;
		}
		last_frame_time = current_time;

		if (current_time - last_sprinkle_time > 2000)
		{
			last_sprinkle_time = current_time;
			for (int i = 0; i < 0.05*nrows*ncols; i++)
			{
				int row = rand() % nrows;
				int col = rand() % ncols;
				grid[row][col] = 1 + rand() % 4;
			}
		}

		/*
		update
		*/
		update(grid, scratch_grid, nrows, ncols);
	
		/*
		draw
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//draw grid

		draw_Grid(grid, nrows, ncols, screen_w, screen_h);
		
		SDL_RenderPresent(renderer);
	}

	return 0;
}