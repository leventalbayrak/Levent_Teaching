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

SDL_Renderer *renderer = NULL;
int screen_width = 800;
int screen_height = 600;

unsigned char prev_key_state[256];
unsigned char *keys = NULL;
SDL_Window *window = NULL;


void set_Pixel(unsigned char* buffer, int width, int x, int y, int r, int g, int b, int a)
{
	int first_byte_of_the_pixel = y * width * 4 + x * 4;
	buffer[first_byte_of_the_pixel] = r;
	buffer[first_byte_of_the_pixel + 1] = g;
	buffer[first_byte_of_the_pixel + 2] = b;
	buffer[first_byte_of_the_pixel + 3] = a;
}

//Alternative example
struct Pixel
{
	unsigned char r, g, b, a;
};
void set_Pixel_Alternative(unsigned char* buffer, int width, int x, int y, int r, int g, int b, int a)
{
	Pixel *p = (Pixel*)buffer;
	p[y*width + x].r = r;
	p[y*width + x].g = g;
	p[y*width + x].b = b;
	p[y*width + x].a = a;
}

void fill_Rectangle(unsigned char*buffer, int buffer_width, int buffer_height, int rect_x, int rect_y, int rect_w, int rect_h, int r, int g, int b, int a)
{
	for (int i = 0; i < rect_w; i++)
	{
		for (int j = 0; j < rect_h; j++)
		{
			int x = i + rect_x;
			int y = j + rect_y;
			set_Pixel_Alternative(buffer, screen_width, x, y, r, g, b, a);
		}
	}
}

void greyscale(float * dst, unsigned char * src, int w, int h)
{
	for (int i = 0; i < screen_width*screen_height; i++)
	{
		dst[i] = src[i * 4] + src[i * 4 + 1] + src[i * 4 + 2] / (3 * 255);
	}
}

void recolor(unsigned char * dst, float * src, int w, int h)
{
	for (int i = 0; i < w*h; i++)
	{
		dst[i * 4] = src[i] * 255;
		dst[i * 4 + 1] = src[i] * 255;
		dst[i * 4 + 2] = src[i] * 255;
	}
}

void drawLine(unsigned char* buffer, float x1, float y1, float x2, float y2, int r, int g, int b, int a)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float k, l;
	if (x1 < x2)
	{
		k = x1;
		l = x2;
	}
	else if (x1 == x2)
	{
		if (y1 < y2)
		{
			k = y1;
			l = y2;
		}
		else
		{
			k = y2;
			l = y1;
		}
		for (int i = k; i < l; i++)
		{
			int x = x1;
			int y = i;
			set_Pixel_Alternative(buffer, screen_width, x, y, r, g, b, a);
		}
	}
	else
	{
		k = x2;
		l = x1;
	}
	for (int i = k; i < l; i++)
	{
		int x = i;
		int y = y1 + dy * (i - x1) / dx;
		set_Pixel_Alternative(buffer, screen_width, x, y, r, g, b, a);
	}
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	prev_key_state[256];
	keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	window = SDL_CreateWindow(
		"I did eet",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_SOFTWARE);

	SDL_Surface *your_draw_buffer = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *screen = SDL_GetWindowSurface(window);
	SDL_SetSurfaceBlendMode(your_draw_buffer, SDL_BLENDMODE_NONE);
	unsigned char *my_own_buffer = (unsigned char*)malloc(sizeof(unsigned char)*screen_width*screen_height * 4);
	float *data = new float[screen_width * screen_height];

	float xb = 390;
	float yb = 290;
	float xrp = 770;
	float yrp = 270;
	float xlp = 10;
	float ylp = 270;
	float fx = 1;
	float fy = 1;
	int bsize = 20;
	int paddleh = 60;
	int paddlew = 20;

	for (;;)
	{
		memcpy(prev_key_state, keys, 256);

		//clear screen
		for (int i = 0; i < screen_width*screen_height; i++)
		{
			my_own_buffer[i * 4] = 0;
			my_own_buffer[i * 4 + 1] = 0;
			my_own_buffer[i * 4 + 2] = 0;
			my_own_buffer[i * 4 + 3] = 0;
		}

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		//ball boundries
		if (xb <= 0 || xb >= screen_width - bsize)
		{
			fx *= -1;
		}
		if (yb <= 0 || yb >= screen_height - bsize)
		{
			fy *= -1;
		}

		//left paddle cols
		if (xb <= xlp + paddlew && yb + bsize <= ylp + paddleh && yb + paddleh <= ylp + paddleh)
		{
			fx *= -1;
		}

		//right paddle cols
		if (xb + bsize >= xrp && yb + bsize >= yrp && yb + bsize <= yrp + paddleh)
		{
			fx *= -1;
		}

		xb += fx;
		yb += fy;

		if (xb < screen_width * .5 - .5 * bsize)
		{
			ylp = yb;
		}
		else
		{
			yrp = yb;
		}

		//clip paddles
		if (ylp > screen_height - paddleh)
		{
			ylp = screen_height - paddleh;
		}
		if (ylp < paddleh)
		{
			ylp = paddleh;
		}
		if (yrp > screen_height - paddleh)
		{
			yrp = screen_height - paddleh;
		}
		if (yrp < paddleh)
		{
			yrp = paddleh;
		}


		//left 
		fill_Rectangle(my_own_buffer, screen_width, screen_height, xlp, ylp, paddlew, paddleh, 255, 0, 0, 255);

		//right
		fill_Rectangle(my_own_buffer, screen_width, screen_height, xrp, yrp, paddlew, paddleh, 255, 0, 0, 255);

		//ball
		fill_Rectangle(my_own_buffer, screen_width, screen_height, xb, yb, bsize, bsize, 255, 0, 0, 255);

		memcpy(your_draw_buffer->pixels, my_own_buffer, sizeof(unsigned char)*screen_width*screen_height * 4);

		//BLIT BUFFER TO SCREEN
		SDL_BlitScaled(your_draw_buffer, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
	}
	return 0;
}