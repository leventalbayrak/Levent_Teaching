#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//download SDL and SDL image

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
//copy the SDL_image.h from the SDL image folder to your SDL include folder
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
//copy the SDL2_image.lib from the SDL image folder to your SDL lib folder
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")


namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 960;
	int screen_height = 768;
}

int main(int argc, char **argv)
{
	//place the png file next to your main.cpp
	char *image_filename = "test.png";
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(0));

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	//load image into a surface.
	SDL_Surface *sprite_surface = IMG_Load(image_filename);
	assert(sprite_surface);
	//create a texture from the surface. The texture is an GPU-efficient/compatible version of the surface
	//we draw textures, not surfaces.
	SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(Game::renderer, sprite_surface);
	//free the surface as it is not needed anymore
	SDL_FreeSurface(sprite_surface);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	bool done = false;
	while (!done)
	{
		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		/*
		GAME CODE
		*/

		//every frame
		
		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		//define the rectangle you are copying FROM the texture
	//in this case, it is a 32x32 box in the top left corner of the image
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = 32;
		src.h = 32;

		//define the rectangle you are copying TO on the screen
		//you can set width and height to anything you want, the GPU will properly scale your tile
		SDL_Rect dest;
		dest.x = Game::screen_width / 2;
		dest.y = Game::screen_height / 2;
		dest.w = 64;
		dest.h = 64;

		//set transparency of the texture.
		//0 to 255, 255 is opaque
		SDL_SetTextureAlphaMod(sprite_texture, 255);

		//copy from source texture to destination screen.
		//you can change the rotation angle and provide a pivot point for rotations.
		//SDL_FLIP_XXX enumeration allows you to mirror the image
		SDL_RenderCopyEx(Game::renderer, sprite_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}

