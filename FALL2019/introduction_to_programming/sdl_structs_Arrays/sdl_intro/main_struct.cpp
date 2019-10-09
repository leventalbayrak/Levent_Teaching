#include <stdio.h>

#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)

struct Box
{
	float posx, posy;
	float velx, vely;
	float size;
};

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int screen_width = 800;
	int screen_height = 600;
	SDL_Window* window = SDL_CreateWindow("LOOP", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//posx, posy, velx, vely, size
	Box box1 = 
	{ 
		screen_width / 2 , screen_height / 2, //pos
		2.0, 1.3, //vel
		16 //size
	};
	Box box2 = { screen_width / 2 , screen_height / 2, 1.8, 1.3, 16 };

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
		box1.posx += box1.velx * dt;
		box1.posy += box1.vely * dt;

		/*
		correct velocity due to collision
		*/
		if (box1.posx < 0 || box1.posx + box1.size > screen_width)
		{
			box1.velx *= -1.0f;
		}

		if (box1.posy < 0 || box1.posy + box1.size > screen_height)
		{
			box1.vely *= -1.0f;
		}

	
		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect rectangle = { 0 };
		rectangle.x = box1.posx;
		rectangle.y = box1.posy;
		rectangle.w = box1.size;
		rectangle.h = box1.size;

		SDL_SetRenderDrawColor(renderer, 55, 255, 153, 255);
		SDL_RenderFillRect(renderer, &rectangle);


		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}








	return 0;
}