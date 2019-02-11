#pragma warning(disable : 4996)

#include "SDL2-2.0.8\include\SDL.h"
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

#include "draw.h"
#include "collision.h"

void wmain()
{
	srand(time(0));
	SDL_Init(SDL_INIT_VIDEO);

	int screen_width = 800;
	int screen_height = 600;
	int pixelN = screen_width * screen_height;
	SDL_Window *window = SDL_CreateWindow("Always a test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *window_surface = SDL_GetWindowSurface(window);
	SDL_Surface *work_surface = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
	
	Color_Set black;
	black.r = 0;
	black.g = 0;
	black.b = 0;

	static Color_Set *p = (Color_Set *)work_surface->pixels;
	Color_Set *canvas = (Color_Set*)calloc(pixelN, sizeof(Color_Set));
	for (int i = 0; i < pixelN; i++) canvas[i].a = 255;

	int performance_timer_start = SDL_GetTicks();
	int performance_timer_end = 0;
	int performance_counter = 0;
	float performance_count_rate = 100.0;

	Entity *border = (Entity*)malloc(sizeof(Entity) * 4);
	border[0].body.size.x = screen_width;
	border[0].body.size.y = 10;
	border[0].body.position.x = 0;
	border[0].body.position.y = -border[0].body.size.y;
	border[0].mass = 1.0;

	border[1].body.size.x = screen_width;
	border[1].body.size.y = 10;
	border[1].body.position.x = 0;
	border[1].body.position.y = screen_height - border[1].body.size.y;
	border[1].mass = 1.0;

	border[2].body.size.x = 10;
	border[2].body.size.y = screen_height;
	border[2].body.position.x = -border[2].body.size.x;
	border[2].body.position.y = 0;
	border[2].mass = 1.0;

	border[3].body.size.x = 10;
	border[3].body.size.y = screen_height;
	border[3].body.position.x = screen_width - border[3].body.size.x;
	border[3].body.position.y = 0;
	border[3].mass = 1.0;

	int boxN = 4;
	float box_sizes = 100.0;
	Entity *box = (Entity*)malloc(sizeof(Entity) * boxN);
	for (int i = 0; i < boxN; i++)
	{
		box[i].body.size.x = box_sizes;
		box[i].body.size.y = box_sizes;
		box[i].body.position.x = rand() % (screen_width - (int)box[i].body.size.x);
		box[i].body.position.y = rand() % (screen_height - (int)box[i].body.size.y);
		box[i].velocity.x = 1.0 - 2.0 * rand() / RAND_MAX;
		box[i].velocity.y = 1.0 - 2.0 * rand() / RAND_MAX;
		box[i].mass = 1.0;
		box[i].body.color.r = rand() % 256;
		box[i].body.color.g = rand() % 256;
		box[i].body.color.b = rand() % 256;
		box[i].body.color.a = 255;
		int random = rand() % 3;
		if (random == 0) box[i].body.color.r = 255;
		else if (random == 1) box[i].body.color.g = 255;
		else box[i].body.color.b = 255;
	}

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT) exit(0);
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) exit(0);
		}

		for (int i = 0; i < boxN; i++)
		{
			for (int j = i + 1; j < boxN; j++) impulse(box[i].body.position, box[i].body.size, &box[i].velocity, box[i].mass, box[j].body.position, box[j].body.size, &box[j].velocity, box[j].mass);
			for (int j = 0; j < 4; j ++) impulse(box[i].body.position, box[i].body.size, &box[i].velocity, box[i].mass, border[j].body.position, border[j].body.size, &border[j].velocity, border[j].mass);
			fp_Add(&box[i].body.position, box[i].velocity);
		}


		for (int i = 0; i < pixelN; i++)
		{
			color_Copy(&canvas[i], black);
		}

		for (int i = 0; i < boxN; i++) fill_Rect(box[i].body, canvas, screen_width, screen_height);

		memcpy(p, canvas, sizeof(Color_Set) * pixelN);
		SDL_BlitScaled(work_surface, NULL, window_surface, NULL);

		SDL_UpdateWindowSurface(window);

		if (++performance_counter >= performance_count_rate)
		{
			performance_timer_end = SDL_GetTicks();
			float rate = (performance_timer_end - performance_timer_start)/ performance_count_rate;
			printf("%fms per frame\n", rate);
			performance_timer_start = SDL_GetTicks();
			performance_counter = 0;
		}
	}
}