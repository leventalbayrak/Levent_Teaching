#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

struct Pixel
{
	unsigned char r, g, b;
};

namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;

	unsigned char prev_key_state[256];
	unsigned char *keys = NULL;
	SDL_Window *window = NULL;

	SDL_Surface *scratch;
	SDL_Surface *screen;

	int *grid = NULL;
	int *px, *py;
	Pixel *color;
	int n_walkers = 100;

	void init()
	{
		SDL_Init(SDL_INIT_VIDEO);

		prev_key_state[256];
		keys = (unsigned char*)SDL_GetKeyboardState(NULL);

		window = SDL_CreateWindow(
			"Fortnite",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screen_width, screen_height, SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window,
			-1, SDL_RENDERER_SOFTWARE);


		scratch = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 24, SDL_PIXELFORMAT_RGB24);

		screen = SDL_GetWindowSurface(window);

		grid = (int*)malloc(scratch->w*scratch->h * sizeof(int));
		memset(grid, 0, sizeof(int)*scratch->w*scratch->h);

		px = (int*)malloc(n_walkers * sizeof(int));
		py = (int*)malloc(n_walkers * sizeof(int));
		color = (Pixel*)malloc(n_walkers * sizeof(Pixel));

		for (int i = 0; i < n_walkers; i++)
		{
			px[i] = rand() % screen_width;
			py[i] = rand() % screen_height;
			color[i].r = rand() % 256;
			color[i].g = rand() % 256;
			color[i].b = rand() % 256;
		}
		color[0].r = 0;
		color[0].g = 0;
		color[0].b = 0;
		
	}


	void update()
	{

		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		for (int i = 0; i < n_walkers; i++)
		{

			int k = rand() % 4;
			if (k == 0)
			{
				py[i]--;
				if (py[i] < 0) py[i] = 0;
			}
			else if (k == 1)
			{
				px[i]++;
				if (px[i] > screen_width - 1) px[i] = screen_width - 1;
			}
			else if (k == 2)
			{
				py[i]++;
				if (py[i] > screen_height - 1) py[i] = screen_height - 1;
			}
			else if (k == 3)
			{
				px[i]--;
				if (px[i] < 0) px[i] = 0;
			}

			grid[py[i] *screen_width + px[i]] = i;
		}
	}


	void draw_1()
	{
		//clear screen with white
		for (int i = 0; i < scratch->w*scratch->h; i++)
		{
			Pixel *p = (Pixel *)scratch->pixels;

			p[i].r = color[grid[i]].r;
			p[i].g = color[grid[i]].g;
			p[i].b = color[grid[i]].b;
		}

		SDL_BlitScaled(scratch, NULL, screen, NULL);

		SDL_UpdateWindowSurface(window);

	}

}
int main(int argc, char **argv)
{
	

	Game::init();


	for(;;)
	{
		Game::update();
		
		Game::draw_1();
	}



	return 0;
}