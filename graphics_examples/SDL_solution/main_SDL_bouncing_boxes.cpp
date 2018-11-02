#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

struct Box_Pos
{
	float x, y;
};
struct Box_Size
{
	float w, h;
};
struct Box_Speed
{
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

	int n_boxes = 2000;
	Box_Pos *box_pos = new Box_Pos[n_boxes];
	Box_Size *box_size = new Box_Size[n_boxes];
	Box_Speed *box_speed = new Box_Speed[n_boxes];
	
	float speed_coefficient = 1;

	for (int i = 0; i < n_boxes; i++)
	{
		box_pos[i].x = screen_width/2;
		box_pos[i].y = screen_height / 2;

		box_size[i].w = 4 + rand() % 9;
		box_size[i].h = box_size[i].w;

		box_speed[i].sx = speed_coefficient * (1.0 - 2.0*rand() / RAND_MAX);
		box_speed[i].sy = speed_coefficient * (1.0 - 2.0*rand() / RAND_MAX);
	}
	
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
		for (int i = 0; i < n_boxes; i++)
		{
			box_pos[i].x += box_speed[i].sx;
			box_pos[i].y += box_speed[i].sy;

			if (box_pos[i].x < 0 || box_pos[i].x + box_size[i].w > screen_width)
			{
				box_speed[i].sx *= -1.0;
			}
			if (box_pos[i].y < 0 || box_pos[i].y + box_size[i].h > screen_height)
			{
				box_speed[i].sy *= -1.0;
			}
		}
		

		
		
		//RENDER
		
		//set color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(renderer);

		//set color to red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		for (int i = 0; i < n_boxes; i++)
		{
			SDL_Rect rect;
			rect.x = box_pos[i].x;
			rect.y = box_pos[i].y;
			rect.w = box_size[i].w;
			rect.h = box_size[i].h;

			//draw filled rectangle in the backbuffer
			SDL_RenderFillRect(renderer, &rect);
		}

		//flip buffers
		SDL_RenderPresent(renderer);

	}



	return 0;
}