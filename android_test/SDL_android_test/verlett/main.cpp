

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

SDL_Renderer* renderer = NULL;
int screen_w = 0;
int screen_h = 0;

struct Box
{
	float size;
	float* x, * y, * vx, * vy;
	unsigned char* active;
	int array_size;
};

void init(Box* p, int array_size)
{
	p->size = 16.0;
	p->array_size = array_size;
	p->active = (unsigned char*)calloc(array_size, sizeof(unsigned char));
	p->x = (float*)calloc(array_size, sizeof(float));
	p->y = (float*)calloc(array_size, sizeof(float));
	p->vx = (float*)calloc(array_size, sizeof(float));
	p->vy = (float*)calloc(array_size, sizeof(float));
}

void update(Box* p)
{
	for (int i = 0; i < p->array_size; i++)
	{
		if (p->active[i] != 0)
		{
			p->x[i] += p->vx[i];
			p->y[i] += p->vy[i];
		}
	}
}

void draw(Box* p)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
	for (int i = 0; i < p->array_size; i++)
	{
		if (p->active[i] != 0)
		{
			SDL_Rect rect = { p->x[i],p->y[i],p->size,p->size };
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

int spawn(Box* p)
{
	for (int i = 0; i < p->array_size; i++)
	{
		if (p->active[i] == 0)
		{
			return i;
		}
	}

	return -1;
}

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

	Box boxes;
	init(&boxes, 1000);

	for (;;)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
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
			else if (e.type == SDL_FINGERUP)
			{
				float mx = e.tfinger.x * screen_w;
				float my = e.tfinger.y * screen_h;

				int k = spawn(&boxes);
				if (k != -1)
				{
					boxes.active[k] = 1;
					boxes.x[k] = mx;
					boxes.y[k] = my;
					boxes.vx[k] = 2.0 * (1.0 - 2.0 * rand() / RAND_MAX);
					boxes.vy[k] = 2.0 * (1.0 - 2.0 * rand() / RAND_MAX);
				}

			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				float mx = e.button.x;
				float my = e.button.y;

				int k = spawn(&boxes);
				if (k != -1)
				{
					boxes.active[k] = 1;
					boxes.x[k] = mx;
					boxes.y[k] = my;
					boxes.vx[k] = 2.0 * (1.0 - 2.0 * rand() / RAND_MAX);
					boxes.vy[k] = 2.0 * (1.0 - 2.0 * rand() / RAND_MAX);
				}

			}
		}

		update(&boxes);
		for (int i = 0; i < boxes.array_size; i++)
		{
			if (boxes.active[i] != 0)
			{
				if (boxes.x[i] < 0)
				{
					boxes.vx[i] *= -1;
				}
				if (boxes.y[i] < 0)
				{
					boxes.vy[i] *= -1;
				}
				if (boxes.x[i] > screen_w - boxes.size)
				{
					boxes.vx[i] *= -1;
				}
				if (boxes.y[i] > screen_h - boxes.size)
				{
					boxes.vy[i] *= -1;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		draw(&boxes);

		SDL_RenderPresent(renderer);


	}

	return 0;
}