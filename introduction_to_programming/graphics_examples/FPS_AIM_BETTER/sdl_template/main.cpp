#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//SDL header
#include "SDL2-2.0.9/include/SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

SDL_Renderer *renderer = NULL;

//given the position of the target, draw the target (red) and bullseye square (white)
void draw_Target(int x, int y, int target_size, int bullseye_size)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = target_size;
	rect.h = target_size;
	SDL_SetRenderDrawColor(renderer, 240, 10, 8, 255);
	SDL_RenderFillRect(renderer, &rect);

	rect.x += (target_size - bullseye_size) / 2;
	rect.y += (target_size - bullseye_size) / 2;
	rect.w = bullseye_size;
	rect.h = bullseye_size;
	SDL_SetRenderDrawColor(renderer, 12, 242, 7, 255);
	SDL_RenderFillRect(renderer, &rect);

}

int check_Hit(int click_X, int click_Y, int target_X, int target_Y, int target_w, int target_h)
{
	if (click_X < target_X) return 0;
	if (click_X > target_X + target_w) return 0;
	if (click_Y < target_Y) return 0;
	if (click_Y > target_Y + target_h) return 0;

	return 1;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	srand(time(0));

	int screen_width = 640;
	int screen_height = 480;
	SDL_Window *window = SDL_CreateWindow("xXxKILLERBEEZZZxXx", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	int current_mouse_x = 0;
	int current_mouse_y = 0;

	int target_position_x = screen_width / 2;
	int target_position_y = screen_height / 2;
	int target_size = 20;
	int bullseye_size = 4;
	unsigned int last_time_target_moved = SDL_GetTicks();

	unsigned int time_game_started = 0;
	int game_started = 0;

	int number_of_clicks = 0;
	int number_of_targets_hit = 0;

	float accuracy = 0.0;

	//infinite loop
	for (;;)
	{
		int mouse_clicked = 0;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				//terminate the process
				exit(0);
			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				mouse_clicked = 1;
				current_mouse_x = event.button.x;
				current_mouse_y = event.button.y;
			}
		}

		unsigned int current_time = SDL_GetTicks();

		if (mouse_clicked == 1 && game_started == 0)
		{
			game_started = 1;
			time_game_started = current_time;
		}

		if (game_started == 1)
		{
			if (mouse_clicked == 1)
			{
				number_of_clicks++;

				if (check_Hit(current_mouse_x, current_mouse_y, target_position_x, target_position_y, target_size, target_size) == 1)
				{
					number_of_targets_hit++;

					float distance_x = current_mouse_x - (target_position_x+0.5*target_size);
					distance_x *= distance_x;

					float distance_y = current_mouse_y - (target_position_y + 0.5*target_size);
					distance_y *= distance_y;

					accuracy += sqrt(distance_x + distance_y);

					printf("you hit the target\n");
					target_position_x = (screen_width - target_size) * rand() / RAND_MAX;
					target_position_y = (screen_height - target_size) * rand() / RAND_MAX;
					last_time_target_moved = current_time;
					printf("target moved - timeout\n");
				}
			}

			if (current_time - last_time_target_moved > 1000)
			{
				target_position_x = (screen_width - target_size) * rand() / RAND_MAX;
				target_position_y = (screen_height - target_size) * rand() / RAND_MAX;
				last_time_target_moved = current_time;
				printf("target moved - timeout\n");
			}

			if (current_time - time_game_started > 30000)
			{
				break;
			}

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			draw_Target(target_position_x, target_position_y, target_size, bullseye_size);
		}


		SDL_RenderPresent(renderer);
	}

	printf("TIME IS UP!\n");
	printf("number of clicks: %d\n", number_of_clicks);
	printf("number of targets hit: %d\n", number_of_targets_hit);
	
	float precision = 100.0*number_of_targets_hit / number_of_clicks;
	printf("precision: %.2f\%\n", precision);

	accuracy /= number_of_targets_hit;
	printf("accuracy: %f\n", accuracy);

	getchar();
	return 0;
}