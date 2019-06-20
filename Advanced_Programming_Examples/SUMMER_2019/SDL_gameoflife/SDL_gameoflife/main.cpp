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
int screen_w = 800;
int screen_h = 600;

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

	/*
	once above task is completed,
	copy scratch grid into grid
	*/

}

void randomize_Grid(int **grid, int nrows, int ncols)
{
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			grid[i][j] = rand() % 2;
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

			if (grid_value == 0)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}

			SDL_Rect rect = { cell_x,cell_y,cell_w,cell_h };
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("conway's game of life", 50, 50, screen_w, screen_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(0));

	//allocate grid
	int nrows = screen_h / 10;
	int ncols = screen_w / 10;
	int **grid = (int **)malloc(sizeof(int*)*nrows);
	for (int i = 0; i < nrows; i++)
	{
		grid[i] = (int *)malloc(sizeof(int)*ncols);
	}

	//randomize grid
	randomize_Grid(grid, nrows, ncols);

	unsigned int last_frame_time = SDL_GetTicks();

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
					randomize_Grid(grid, nrows, ncols);
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

		/*
		update
		*/

	
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