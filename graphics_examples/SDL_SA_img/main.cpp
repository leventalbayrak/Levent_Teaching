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


Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;

	float temperature = 1.0;
	float temperature_decay = 0.98;

	const int n_boxes = 500;
	SDL_Rect boxes[n_boxes];
	SDL_Rect tmp_boxes[n_boxes];
	unsigned int color[n_boxes];

	unsigned char prev_key_state[256];
	unsigned char *keys = NULL;
	SDL_Window *window;

	void init()
	{
		SDL_Init(SDL_INIT_VIDEO);

		prev_key_state[256];
		keys = (unsigned char*)SDL_GetKeyboardState(NULL);

		window = SDL_CreateWindow(
			"Fortnite",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

		Game::renderer = SDL_CreateRenderer(window,
			-1, SDL_RENDERER_SOFTWARE);


		SDL_Surface *original = IMG_Load("image.png");
		SDL_Surface *surface_current = SDL_CreateRGBSurfaceWithFormat(0, original->w, original->h, 32, SDL_PIXELFORMAT_RGBA32);
		SDL_Surface *surface_tmp = SDL_CreateRGBSurfaceWithFormat(0, original->w, original->h, 32, SDL_PIXELFORMAT_RGBA32);

		
		
		SDL_Surface* screen = SDL_GetWindowSurface(window);

		unsigned int color0 = SDL_MapRGBA(surface_current->format, 255, 255, 255, 255);
		unsigned int color1 = SDL_MapRGBA(surface_current->format, 255, 0, 0, 100);
		unsigned int color2 = SDL_MapRGBA(surface_current->format, 0, 255, 0, 50);

		SDL_SetSurfaceBlendMode(surface_current, SDL_BLENDMODE_BLEND);
		//printf("%s\n", SDL_GetPixelFormatName(surface_current->format->format));
		//getchar();

		SDL_Rect wipe_rect = { 0,0,200,200 };
		
		SDL_FillRect(surface_tmp, &wipe_rect, color0);
		SDL_Rect rect0 = { 10,10,100,100 };
		SDL_FillRect(surface_tmp, &rect0, color0);
		SDL_Rect src_rect = { 0,0,100,100 };
		SDL_BlitSurface(surface_tmp, &src_rect, surface_current, &rect0);
		
		SDL_FillRect(surface_tmp, &wipe_rect, color0);
		SDL_Rect rect1 = { 10,10,50,50 };
		SDL_FillRect(surface_tmp, &rect1, color1);
		src_rect = { 0,0,50,50 };
		SDL_BlitSurface(surface_tmp, &src_rect, surface_current, &rect1);
		
		SDL_FillRect(surface_tmp, &wipe_rect, color0);
		SDL_Rect rect2 = { 20,20,50,50 };
		SDL_FillRect(surface_tmp, &rect2, color2);
		src_rect = { 0,0,50,50 };
		SDL_BlitSurface(surface_tmp, &src_rect, surface_current, &rect2);
		
	
	//	int r = SDL_SetSurfaceBlendMode(screen, SDL_BLENDMODE_BLEND);
	//	SDL_FillRect(surface_current, &rect0, color0);
		SDL_Rect rect = {};
		SDL_BlitSurface(surface_current, 0, screen, &rect);

	}

	void update()
	{

		memcpy(Game::prev_key_state, Game::keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

	
	}

	void draw()
	{

		//clear screen with white
		//SDL_RenderClear(Game::renderer);

		SDL_UpdateWindowSurface(window);

		//flip buffers
		//SDL_RenderPresent(Game::renderer);
	}

}
int main(int argc, char **argv)
{
	

	Game::init();


	for(;;)
	{
		Game::update();
		
		Game::draw();
	}



	return 0;
}