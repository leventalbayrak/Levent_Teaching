#include <stdio.h>
#include <stdlib.h>

#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)

SDL_Renderer* renderer = NULL;

void draw_Square(float x, float y, float size)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = size;
	rect.h = size;

	SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int screen_width = 800;
	int screen_height = 600;
	SDL_Window* window = SDL_CreateWindow("LOOP", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	for (;;)
	{
		/*
		consume events
		*/
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				printf("GOODBYE\n");
				exit(0);
			}
		}

		/*
		game code here
		*/

		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 180, 185, 180, 255);
		SDL_RenderClear(renderer);


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		draw_Square(50, 50, 50);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		draw_Square(52, 52, 46);

		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}








	return 0;
}