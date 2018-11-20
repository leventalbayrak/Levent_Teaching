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

	SDL_Surface *original;
	SDL_Surface *tmp;
	SDL_Surface *scratch;
	SDL_Surface *screen;

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


		original = IMG_Load("mickey_mouse.jpg");
		printf("%d\n", original->format->BitsPerPixel);
		tmp = SDL_CreateRGBSurfaceWithFormat(0, original->w, original->h, 24, SDL_PIXELFORMAT_RGB24);
		scratch = SDL_CreateRGBSurfaceWithFormat(0, original->w, original->h, 24, SDL_PIXELFORMAT_RGB24);

		screen = SDL_GetWindowSurface(window);
	}

	void effect_1()
	{

		for (int i = 0; i < tmp->w*tmp->h; i++)
		{
			Pixel *original_pixel = (Pixel *)original->pixels;

			Pixel *data = (Pixel *)tmp->pixels;

			if (original_pixel[i].r != data[i].r) data[i].r = rand() % 256;
			if (original_pixel[i].g != data[i].g) data[i].g = rand() % 256;
			if (original_pixel[i].b != data[i].b) data[i].b = rand() % 256;

		}
	}

	void effect_2()
	{

		for (int i = 0; i < tmp->w*tmp->h; i++)
		{
			Pixel *original_pixel = (Pixel *)original->pixels;

			Pixel *data = (Pixel *)tmp->pixels;

			double s = (double)original_pixel[i].r + original_pixel[i].g + original_pixel[i].b;
			data[i].r = s / 3.0;
			data[i].g = s / 3.0;
			data[i].b = s / 3.0;
		}
	}

	void effect_3()
	{
		
		for (int i = 0; i < tmp->w*tmp->h; i++)
		{
			Pixel *original_pixel = (Pixel *)original->pixels;

			Pixel *data = (Pixel *)tmp->pixels;

			double s = (double)original_pixel[i].r + original_pixel[i].g + original_pixel[i].b;
			data[i].r = s / 3.0;
			data[i].g = s / 3.0;
			data[i].b = s / 3.0;
		}

		double _min = 255;
		double _max = 0;
		for (int i = 0; i < tmp->w*tmp->h; i++)
		{
			Pixel *data = (Pixel *)tmp->pixels;
			if (data[i].r < _min) _min = data[i].r;
			if (data[i].r > _max) _max = data[i].r;
		}

		for (int i = 0; i < tmp->w*tmp->h; i++)
		{
			Pixel *data = (Pixel *)tmp->pixels;
			data[i].r -= _min;
			data[i].g -= _min;
			data[i].b -= _min;

			data[i].r = 255.0*data[i].r / (_max - _min);
			data[i].g = 255.0*data[i].g / (_max - _min);
			data[i].b = 255.0*data[i].b / (_max - _min);
		}


	}


	void effect_4()
	{

		for (int i = 0; i < scratch->w*scratch->h; i++)
		{
			Pixel *original_pixel = (Pixel *)original->pixels;

			Pixel *data = (Pixel *)scratch->pixels;

			double s = (double)original_pixel[i].r + original_pixel[i].g + original_pixel[i].b;
			data[i].r = s / 3.0;
			data[i].g = s / 3.0;
			data[i].b = s / 3.0;
		}

		Pixel *output = (Pixel *)tmp->pixels;
		for (int i = 0; i < scratch->h; i++)
		{
			Pixel *data = (Pixel *)scratch->pixels;
			for (int j = 1; j < scratch->w; j++)
			{
				if (abs(data[i*scratch->w + j-1].r - data[i*scratch->w + j].r) > 12)
				{
					output[i*scratch->w + j].r = data[i*scratch->w + j].r;
					output[i*scratch->w + j].g = data[i*scratch->w + j].r;
					output[i*scratch->w + j].b = data[i*scratch->w + j].r;
				}
				else
				{
					output[i*scratch->w + j].r = 0;
					output[i*scratch->w + j].g = 0;
					output[i*scratch->w + j].b = 0;
				}
			}
		}

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

		effect_4();
	
	}

	void draw_2()
	{
		//clear screen with white

		SDL_Rect dest0 = { 0,screen_height / 4,screen_width/2,screen_height/2 };
		SDL_Rect dest1 = { screen_width / 2,screen_height / 4,screen_width / 2,screen_height / 2 };

		SDL_BlitScaled(original, NULL, screen, &dest0);
		SDL_BlitScaled(tmp, NULL, screen, &dest1);

		SDL_UpdateWindowSurface(window);

	}

	void draw_1()
	{
		//clear screen with white

		SDL_BlitScaled(tmp, NULL, screen, NULL);

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