#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)

#define ZERO {0}
int screen_width = 1280;
int screen_height = 800;
SDL_Renderer* renderer = NULL;

struct f2
{
	float x, y;
};
struct c3
{
	unsigned char r, g, b;
};

namespace Components
{
	const int maxsize = 10000;
	f2 pos[maxsize];
	f2 vel[maxsize];
	f2 size[maxsize];
	c3 color[maxsize];
	float health[maxsize];
}


namespace Entity
{
	const int maxcomponents = 256;
	struct Entity
	{
		unsigned char components[maxcomponents];
	};
	
	Entity entities[10000] = { 0 };
	int nentities = 0;
};

void make_Moving_Box(f2* pos, f2* vel, f2* size)
{
	Entity::entities[Entity::nentities].components[0] = 1;
	Entity::entities[Entity::nentities].components[1] = 1;
	Entity::entities[Entity::nentities].components[2] = 1;

	Components::pos[Entity::nentities] = *pos;
	Components::vel[Entity::nentities] = *vel;
	Components::size[Entity::nentities] = *size;

	Entity::nentities++;
}

void make_Static_Box(f2* pos, f2* size)
{
	Entity::entities[Entity::nentities].components[0] = 1;
	Entity::entities[Entity::nentities].components[2] = 1;

	Components::pos[Entity::nentities] = *pos;
	Components::size[Entity::nentities] = *size;

	Entity::nentities++;
}

namespace System
{
	void physics()
	{
		for (int i = 0; i < Entity::nentities; ++i)
		{
			if (Entity::entities[i].components[0] == 1 && Entity::entities[i].components[1] == 1)
			{
				Components::pos[i].x += Components::vel[i].x;
				Components::pos[i].y += Components::vel[i].y;
				if (Components::pos[i].x < 0 || Components::pos[i].x > screen_width)
				{
					Components::vel[i].x *= -1.0f;
				}
				if (Components::pos[i].y < 0 || Components::pos[i].y > screen_height)
				{
					Components::vel[i].y *= -1.0f;
				}
			}
		}
	}

	void draw()
	{
		for (int i = 0; i < Entity::nentities; ++i)
		{
			if (Entity::entities[i].components[0] == 1 && Entity::entities[i].components[2] == 1)
			{
				SDL_Rect rect = { Components::pos[i].x,Components::pos[i].y,Components::size[i].x,Components::size[i].y };
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

void type0()
{
	SDL_Init(SDL_INIT_VIDEO);

	srand(time(0));

	
	SDL_Window* window = SDL_CreateWindow("markos", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
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

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					int r = rand() % 2;
					if (r == 0)
					{
						f2 pos = { rand() % screen_width, rand() % screen_height };
						f2 vel = { 1.0f - 2.0f*rand() / RAND_MAX,1.0f - 2.0f*rand() / RAND_MAX };
						f2 size = {2 + rand() % 50,2 + rand() % 50 };
						make_Moving_Box(&pos, &vel, &size);
					}
					else
					{
						f2 pos = { rand() % screen_width, rand() % screen_height };
						f2 size = { 2 + rand() % 50,2 + rand() % 50 };
						make_Static_Box(&pos, &size);
					}
				}

			}
		}

		/*
		game code here
		*/
		int mx, my;
		unsigned int mouse_state = SDL_GetMouseState(&mx, &my);


		System::physics();

		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		System::draw();
		

		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}
}


int main(int argc, char** argv)
{
	type0();

	return 0;
}