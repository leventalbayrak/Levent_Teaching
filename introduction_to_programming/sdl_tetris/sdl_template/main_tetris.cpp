//SDL header
#include "SDL2-2.0.9/include/SDL.h"
#include <stdio.h>

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

SDL_Renderer *renderer = NULL;
const int block_size = 10;

void draw_I(int x, int y)
{
	//draw black outline
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect outline_rect = {x, y, 45, 12};
	SDL_RenderFillRect(renderer, &outline_rect);

	//draw individual boxes
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect block_rect = { x + 1, y + 1, block_size, block_size };
	SDL_RenderFillRect(renderer, &block_rect);

	for (int i = 0; i < 3; i++)
	{
		block_rect.x += block_size + 1;
		SDL_RenderFillRect(renderer, &block_rect);
	}

}

void draw_O(int x, int y)
{
	//draw black outline
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect outline_rect = { x, y, 23, 23};
	SDL_RenderFillRect(renderer, &outline_rect);

	SDL_SetRenderDrawColor(renderer, 244, 232, 104, 255);
	SDL_Rect block_rect = { x + 1, y + 1, block_size, block_size };
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.x += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.x -= block_size + 1;
	block_rect.y += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.x += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);
}

void draw_L(int x, int y)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect outline_rect = { x, y, 12, 34 };
	SDL_RenderFillRect(renderer, &outline_rect);

	outline_rect.x += 11;
	outline_rect.y += 22;
	outline_rect.h = 12;
	SDL_RenderFillRect(renderer, &outline_rect);

	SDL_SetRenderDrawColor(renderer, 255, 0, 40, 255);
	SDL_Rect block_rect = { x+1, y+1, block_size, block_size };
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.y += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.y += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);
	
	block_rect.x += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);
}

void draw_J(int x, int y)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect outline_rect = { x+11, y, 12, 34 };
	SDL_RenderFillRect(renderer, &outline_rect);

	outline_rect.x -= 11;
	outline_rect.y += 22;
	outline_rect.h = 12;
	SDL_RenderFillRect(renderer, &outline_rect);

	SDL_SetRenderDrawColor(renderer, 255, 95, 0, 255);
	SDL_Rect block_rect = { x + 12, y + 1, block_size, block_size };
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.y += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.y += block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);

	block_rect.x -= block_size + 1;
	SDL_RenderFillRect(renderer, &block_rect);
}
int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Tetris Effect VR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//infinite loop
	for (;;)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				//terminate the process
				exit(0);
			}
		}

		//GAME CODE BEGIN

		//DRAWING CODE
		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		SDL_RenderClear(renderer);

		draw_J(50, 50);
		draw_L(100, 50);
		draw_O(150, 55);
		draw_I(200, 60);
	
		//swap buffers
		SDL_RenderPresent(renderer);

		//GAME CODE END
	}










	return 0;
}