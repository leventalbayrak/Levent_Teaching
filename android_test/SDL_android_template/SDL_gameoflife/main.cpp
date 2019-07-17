

/*
just in case you do not understand the rules: https://www.youtube.com/watch?v=FWSR_7kZuYg
http://www.conwaylife.com/wiki/Conway%27s_Game_of_Life
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
*/


#ifdef _WIN32
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
#else
#include <SDL2/SDL.h>
#endif

SDL_Renderer *renderer = NULL;
int screen_w = 0;
int screen_h = 0;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);

	//FULL SCREEN
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
	{
		screen_w = displayMode.w;
		screen_h = displayMode.h;
	}

	SDL_Window* window = SDL_CreateWindow("SDL Android Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	for (;;)
	{
		//Handle events on queue
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				exit(0);
			}
			else if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					screen_w = e.window.data1;
					screen_h = e.window.data2;
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_Rect rect = { 10,10,16,16 };
		SDL_RenderFillRect(renderer, &rect);

		//Update screen
		SDL_RenderPresent(renderer);


	}

	return 0;
}