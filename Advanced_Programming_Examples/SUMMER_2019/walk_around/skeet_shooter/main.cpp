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

#define R_ONE() (float)rand()/RAND_MAX
#define R() (1.0-2.0*rand()/RAND_MAX)

class Obstacle
{
	float x, y, w, h;
	int r, g, b;
public:
	Obstacle()
	{
		x = 32000 * R_ONE();
		y = 32000* R_ONE();
		w = 4 + 32 * R_ONE();
		h = 4 + 32 * R_ONE();
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	}

	void draw(float cam_x, float cam_y)
	{
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_Rect rect = { x - cam_x + screen_w / 2,y - cam_y + screen_h / 2,w,h };
		SDL_RenderFillRect(renderer, &rect);
	}

	int is_Visible(float cam_x, float cam_y)
	{
		if (x + w < (cam_x - screen_w/2)) return 0;
		if (x > (cam_x + screen_w/2)) return 0;
		if (y + h < (cam_y-screen_h/2)) return 0;
		if (y > (cam_y + screen_h/2)) return 0;

		return 1;
	}
};


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("conway's game of life", 50, 50, screen_w, screen_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(0));

	unsigned int current_mouse_state = 0;
	unsigned int prev_mouse_state = 0;

	//1000 pointers pointing at obstacles
	int n_obstacles = 100000;
	Obstacle **obstacles = new Obstacle* [n_obstacles];
	for (int i = 0; i < n_obstacles; i++)
	{
		obstacles[i] = new Obstacle();
	}

	float camera_center_x = 500;
	float camera_center_y = 500;
	float camera_vel_x = 0.0;
	float camera_vel_y = 0.0;
	float camera_speed = 1.0;

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;	
		}

		unsigned int current_time = SDL_GetTicks();

		int mx, my;
		prev_mouse_state = current_mouse_state;
		current_mouse_state = SDL_GetMouseState(&mx, &my);

		float screen_center_x = screen_w / 2;
		float screen_center_y = screen_h / 2;

		float dx = mx - screen_center_x;
		float dy = my - screen_center_y;

		float mag = sqrt(dx*dx + dy * dy);

		float ux = dx / mag;
		float uy = dy / mag;

		if (mag == 0.0)
		{
			ux = 0.0;
			uy = 0.0;
		}

		
		camera_vel_x = 0.0;
		camera_vel_y = 0.0;
		if (mag > 16)
		{
			camera_vel_x = ux * camera_speed;
			camera_vel_y = uy * camera_speed;
		}
		camera_center_x += camera_vel_x;
		camera_center_y += camera_vel_y;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		for (int i = 0; i < n_obstacles; i++)
		{
			if (obstacles[i]->is_Visible(camera_center_x, camera_center_y))
			{
				obstacles[i]->draw(camera_center_x, camera_center_y);
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect parking_rect = { screen_w/2 - 16,screen_h/2 - 16,32,32 };
		SDL_RenderDrawRect(renderer, &parking_rect);

		SDL_RenderPresent(renderer);
	}


	
	getchar();

	return 0;
}