#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)

SDL_Renderer* renderer = NULL;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	srand(time(0));

	int screen_width = 1280;
	int screen_height = 800;
	SDL_Window* window = SDL_CreateWindow("LOOP", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float turret_w = 20;
	float turret_h = 20;
	float turret_x = screen_width/2;
	float turret_y = screen_height - turret_h;

	float bullet_x = 0;
	float bullet_y = 0;
	float bullet_w = 10;
	float bullet_h = 10;
	float bullet_sx = 0;
	float bullet_sy = 0;
	float bullet_ax = 0;
	float bullet_ay = 0;

	float target_w = 20;
	float target_h = 5;
	float target_x = screen_width - target_w*4;
	float target_y = screen_height - target_h;

	int bullet_state = 0;//0 launch, 1 in air
	float gravity = 0.001;

	float solution_ax = 1.0*(1.0 - 2.0*rand() / RAND_MAX); // -1 to 1
	float solution_ay = 1.0*(1.0 - 2.0*rand() / RAND_MAX); // - 1 to 1

	float best_solution_ax = solution_ax;
	float best_solution_ay = solution_ay;
	float best_distance_from_target = 10000000000;

	float temperature = 1.0;

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
		bullet_ax = 0;
		bullet_ay = 0;

		if (bullet_state == 0)
		{

			solution_ax += 0.02*(1.0 - 2.0*rand() / RAND_MAX);
			solution_ay += 0.02*(1.0 - 2.0*rand() / RAND_MAX);
			temperature *= 0.98;

			bullet_x = turret_x;
			bullet_y = turret_y;
			bullet_sx = 0;
			bullet_sy = 0;
			bullet_ax = solution_ax;
			bullet_ay = solution_ay;

			bullet_sx += bullet_ax;
			bullet_sy += bullet_ay;

			bullet_x += bullet_sx;
			bullet_y += bullet_sy;

			bullet_state = 1;
		}
		else if (bullet_state == 1)
		{
			bullet_ay += gravity;
			bullet_sx += bullet_ax;
			bullet_sy += bullet_ay;

			bullet_x += bullet_sx;
			bullet_y += bullet_sy;
		}

		// limit bullet speeeed :D it goes out of the screen
		if (bullet_y > target_y)
		{
			float distance_from_target = sqrt((target_x - bullet_x)*(target_x - bullet_x));
			distance_from_target += sqrt( bullet_sx * bullet_sx + bullet_sy * bullet_sy)*15;

			if (distance_from_target < best_distance_from_target)
			{
				best_distance_from_target = distance_from_target;
				best_solution_ax = solution_ax;
				best_solution_ay = solution_ay;

			}
			else
			{
				float p = (float)rand() / RAND_MAX;

				float z = exp((best_distance_from_target - distance_from_target) / temperature);

				if (p < z)
				{
					best_distance_from_target = distance_from_target;
					best_solution_ax = solution_ax;
					best_solution_ay = solution_ay;
				}
				else
				{
					solution_ax = best_solution_ax;
					solution_ay = best_solution_ay;
				}
			}

			printf("temp: %.2f current: %.2f %.2f %.2f best: %.2f %.2f %.2f\n",
				temperature, 
				solution_ax, solution_ay, distance_from_target, 
				best_solution_ax, best_solution_ay, best_distance_from_target);

			bullet_state = 0;
		}

		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_Rect r = { turret_x, turret_y, turret_w, turret_h };
			SDL_RenderFillRect(renderer, &r);
		}

		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_Rect r = { target_x, target_y, target_w, target_h };
			SDL_RenderFillRect(renderer, &r);
		}

		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_Rect r = { bullet_x, bullet_y, bullet_w, bullet_h };
			SDL_RenderFillRect(renderer, &r);
		}
		
		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}








	return 0;
}