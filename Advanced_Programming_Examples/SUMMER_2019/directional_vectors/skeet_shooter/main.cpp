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

SDL_Renderer *renderer = NULL;
int screen_w = 1280;
int screen_h = 800;

#define R() (1.0-2.0*rand()/RAND_MAX)


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Mark is being chased - but he has a gun...", 50, 50, screen_w, screen_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(0));

	float dt = 1.0;

	float actor_x = 0.0;
	float actor_y = 0.0;
	float actor_speed = 0.1;

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;	
		}

		unsigned int current_time = SDL_GetTicks();
		int mx, my;
		SDL_GetMouseState(&mx, &my);

		float target_x = mx;
		float target_y = my;

		//step 1 - find the distance vector between the source and the target
		float dist_x = target_x - actor_x;
		float dist_y = target_y - actor_y;
		//step 2 - calculate the unit vector - unit vector points in the direction of the target and its magnitude is exactly 1
		float magnitude = sqrt(dist_x * dist_x + dist_y * dist_y);
		float unit_x = dist_x / magnitude;
		float unit_y = dist_y / magnitude;
		//step 3 - calculate velocity vector by multiplying unit vector with the speed
		float vel_x = unit_x * actor_speed;
		float vel_y = unit_y * actor_speed;

		//unrelated physics
		actor_x += vel_x * dt;
		actor_y += vel_y * dt;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		SDL_Rect rect = { actor_x, actor_y, 8,8 };
		SDL_RenderFillRect(renderer, &rect);


		SDL_RenderPresent(renderer);
	}

	getchar();

	return 0;
}