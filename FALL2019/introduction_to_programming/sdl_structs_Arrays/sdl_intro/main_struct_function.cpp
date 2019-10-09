#include <stdio.h>

#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)


SDL_Renderer* renderer = NULL;


struct Box
{
	float posx, posy;
	float velx, vely;
	float size;
	int r, g, b;
};

void update(Box* box, int screenw, int screenh, float dt)
{
	box->posx += box->velx * dt;
	box->posy += box->vely * dt;

	/*
	correct velocity due to collision
	*/
	if (box->posx < 0 || box->posx + box->size > screenw)
	{
		box->velx *= -1.0f;
	}

	if (box->posy < 0 || box->posy + box->size > screenh)
	{
		box->vely *= -1.0f;
	}
}

void draw(Box *box)
{
	SDL_Rect rectangle = { 0 };
	rectangle.x = box->posx;
	rectangle.y = box->posy;
	rectangle.w = box->size;
	rectangle.h = box->size;

	SDL_SetRenderDrawColor(renderer, box->r, box->g, box->b, 255);
	SDL_RenderFillRect(renderer, &rectangle);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int screen_width = 800;
	int screen_height = 600;
	SDL_Window* window = SDL_CreateWindow("LOOP", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//posx, posy, velx, vely, size
	Box box1 = 
	{ 
		screen_width / 2 , screen_height / 2, //pos
		2.0, 1.3, //vel
		16, //size,
		255, 0 , 0 //color
	};

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
		update(&box1, screen_width, screen_height, dt);

	
		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw(&box1);


		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}








	return 0;
}