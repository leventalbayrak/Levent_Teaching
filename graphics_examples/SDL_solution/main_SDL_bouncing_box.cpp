#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

struct Box
{
	float x, y;
	float w, h;
	float sx, sy;
};

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int screen_width = 800;
	int screen_height = 600;

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, 
		-1, SDL_RENDERER_ACCELERATED);


	Box box = {50,50,50,50,0.1,0.12};

	bool done = false;
	while (!done)
	{
		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		/*
		GAME CODE
		*/
		
		//UPDATE
		box.x += box.sx;
		box.y += box.sy;

		if (box.x < 0 || box.x+box.w > screen_width)
		{
			box.sx *= -1.0;
		}
		if (box.y < 0 || box.y+box.h > screen_height)
		{
			box.sy *= -1.0;
		}
		
		//RENDER
		
		//set color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(renderer);

		//set color to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		SDL_Rect rect;
		rect.x = box.x;
		rect.y = box.y;
		rect.w = box.w;
		rect.h = box.h;

		//draw filled rectangle in the backbuffer
		SDL_RenderFillRect(renderer, &rect);

		//flip buffers
		SDL_RenderPresent(renderer);

	}



	return 0;
}