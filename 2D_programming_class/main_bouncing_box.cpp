#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

void fill_Rectangle(unsigned char*buffer, int buffer_width, int buffer_height, int rect_x, int rect_y, int rect_w, int rect_h, int r, int g, int b, int a)
{
	for (int y = 0; y < rect_h; y++)
	{
		for (int x = 0; x < rect_w; x++)
		{
			int index = 4*(rect_y+y) * buffer_width + 4*(x + rect_x);
			buffer[index] = r;
			buffer[index+1] = g;
			buffer[index+2] = b;
			buffer[index+3] = a;
		}
	}

}

SDL_Renderer *renderer = NULL;
int screen_width = 800;
int screen_height = 600;

SDL_Window *window = NULL;

struct Box
{
	float x, y;
	float w, h;
	float speed_x, speed_y;
	int r, g, b, a;
};

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		"WONDERFUL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_SOFTWARE);

	SDL_Surface *your_draw_buffer = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *screen = SDL_GetWindowSurface(window);

	SDL_SetSurfaceBlendMode(your_draw_buffer, SDL_BLENDMODE_NONE);

	unsigned char *my_own_buffer = (unsigned char*)malloc(sizeof(unsigned char)*screen_width*screen_height * 4);

	int n_boxes = 5000;
	Box *box = (Box*)malloc(sizeof(Box)*n_boxes);

	for (int i = 0; i < n_boxes; i++)
	{
		box[i].x = screen_width/2;
		box[i].y = screen_height/2;
		box[i].w = 4;
		box[i].h = 4;
		box[i].r = rand() % 256;
		box[i].g = rand() % 256;
		box[i].b = rand() % 256;
		box[i].a = 255;
		box[i].speed_x = 1.0 - 2.0*rand() / RAND_MAX;
		box[i].speed_y = 1.0 - 2.0*rand() / RAND_MAX;
	}

	for (;;)
	{

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		for (int i = 0; i < n_boxes; i++)
		{
			box[i].x += box[i].speed_x;
			box[i].y += box[i].speed_y;

			if (box[i].x < 0)
			{
				box[i].x = 0;
				box[i].speed_x *= -1;
			}
			if (box[i].x > screen_width - box[i].w)
			{
				box[i].x = screen_width - box[i].w;
				box[i].speed_x *= -1;
			}

			if (box[i].y < 0)
			{
				box[i].y = 0;
				box[i].speed_y *= -1;
			}
			if (box[i].y > screen_height - box[i].h)
			{
				box[i].y = screen_height - box[i].h;
				box[i].speed_y *= -1;
			}

		}

		memset(my_own_buffer, 0, 4 * screen_width*screen_height);

		for (int i = 0; i < n_boxes; i++)
		{
			fill_Rectangle(my_own_buffer, screen_width, screen_height,
				box[i].x, box[i].y, box[i].w, box[i].h, 
				box[i].r, box[i].g, box[i].b, box[i].a);
		}

		memcpy(your_draw_buffer->pixels, my_own_buffer, sizeof(unsigned char)*screen_width*screen_height * 4);

		//BLIT BUFFER TO SCREEN
		SDL_BlitScaled(your_draw_buffer, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
	}

	return 0;
}