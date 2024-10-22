#include <stdio.h>

#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int screen_width = 800;
	int screen_height = 600;
	SDL_Window* window = SDL_CreateWindow("LOOP", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float position_x = screen_width / 2;
	float position_y = screen_height / 2;
	float velocity_x = 2.0;
	float velocity_y = 1.3;
	float size = 16;

	float position_x2 = screen_width / 2;
	float position_y2 = screen_height / 2;
	float velocity_x2 = 1.8;
	float velocity_y2 = 1.3;
	float size2 = 16;

	float dt = 0.1;

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
		physics
		*/
		position_x += velocity_x * dt;
		position_y += velocity_y * dt;

		position_x2 += velocity_x2 * dt;
		position_y2 += velocity_y2 * dt;
		/*
		correct velocity due to collision
		*/
		if (position_x < 0 || position_x + size > screen_width)
		{
			velocity_x *= -1.0f;
		}

		if (position_y < 0 || position_y + size > screen_height)
		{
			velocity_y *= -1.0f;
		}

		if (position_x2 < 0 || position_x2 + size2 > screen_width)
		{
			velocity_x2 *= -1.0f;
		}

		if (position_y2 < 0 || position_y2 + size2 > screen_height)
		{
			velocity_y2 *= -1.0f;
		}

		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect rectangle = { 0 };
		rectangle.x = position_x;
		rectangle.y = position_y;
		rectangle.w = size;
		rectangle.h = size;

		SDL_SetRenderDrawColor(renderer, 55, 255, 153, 255);
		SDL_RenderFillRect(renderer, &rectangle);

		SDL_Rect rectangle2 = { 0 };
		rectangle2.x = position_x2;
		rectangle2.y = position_y2;
		rectangle2.w = size2;
		rectangle2.h = size2;

		SDL_SetRenderDrawColor(renderer, 255, 255, 153, 255);
		SDL_RenderFillRect(renderer, &rectangle2);


		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}








	return 0;
}