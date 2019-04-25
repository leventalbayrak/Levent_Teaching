#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.9\include\SDL.h"
#include "SDL2-2.0.9\include\SDL_image.h"
#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//download SDL and SDL image

//include SDL header
#include "SDL2-2.0.9\include\SDL.h"
//copy the SDL_image.h from the SDL image folder to your SDL include folder
#include "SDL2-2.0.9\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")
//copy the SDL2_image.lib from the SDL image folder to your SDL lib folder
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2_image.lib")

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
	char *image_filename = "run.png";
	//drawing text is the same thing as drawing sprites
	char *font_filename = "font_sheet.png";

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

	//load image into a surface.
	SDL_Surface *font_surface = IMG_Load(font_filename);
	assert(font_surface);
	//create a texture from the surface. The texture is an GPU-efficient/compatible version of the surface
	//we draw textures, not surfaces.
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(Game::renderer, font_surface);
	//free the surface as it is not needed anymore
	SDL_FreeSurface(font_surface);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	unsigned int last_text_change_time = SDL_GetTicks();
	char random_text[17];
	int random_text_size = 32;
	for (int i = 0; i < 16; i++)
	{
		random_text[i] = 'a' + rand() % 26;
	}
	random_text[16] = 0;

	bool done = false;
	while (!done)
	{
		unsigned int current_time = SDL_GetTicks();

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

		//set color to black
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
		//clear screen with black
		SDL_RenderClear(Game::renderer);

		//define the rectangle you are copying FROM the texture
		//in this case, it is a 108 by 140 pixel box in the top left corner of the image
		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = 108;
		src.h = 140;

		//define the rectangle you are copying TO on the SCREENw
		//you can set width and height to anything you want, the GPU will properly scale your tile
		SDL_Rect dest;
		dest.x = Game::screen_width / 2;
		dest.y = 100+Game::screen_height / 2;
		dest.w = 64;
		dest.h = 64;

		//set transparency of the texture.
		//0 to 255, 255 is opaque
		SDL_SetTextureAlphaMod(sprite_texture, 255);

		//copy from source texture to destination screen.
		//you can change the rotation angle and provide a pivot point for rotations.
		//SDL_FLIP_XXX enumeration allows you to mirror the image
		SDL_RenderCopyEx(Game::renderer, sprite_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		if (current_time - last_text_change_time > 1000)
		{
			last_text_change_time = current_time;
			random_text_size = 16 + rand() % 64;
			//refresh random text
			for (int i = 0; i < 16; i++)
			{
				random_text[i] = 'a' + rand() % 26;
			}
			random_text[16] = 0;
		}
		
		float text_x = 50;
		float text_y = Game::screen_height / 2;

		//draw one character at a time
		for (int i = 0; i < 16; i++)
		{
			//define the rectangle you are copying FROM the texture
			//in this case, the font sheet is 1024 by 1024 pixel box and it contains 256 characters
			//characters are laid out on a 16 by 16 grid
			//each character is 64 by 64. This mean you need to divide the ascii character by 16 for row and modulus 16 for column.
			SDL_Rect src;
			src.x = 64 * (random_text[i] % 16);//column
			src.y = 64 * (random_text[i] / 16);//row
			src.w = 64;
			src.h = 64;

			//define the rectangle you are copying TO on the SCREENw
			//you can set width and height to anything you want, the GPU will properly scale your tile
			SDL_Rect dest;
			dest.x = text_x;
			dest.y = text_y;
			dest.w = random_text_size;
			dest.h = dest.w;

			//set transparency of the texture.
			//0 to 255, 255 is opaque
			SDL_SetTextureAlphaMod(font_texture, 255);

			//copy from source texture to destination screen.
			//you can change the rotation angle and provide a pivot point for rotations.
			//SDL_FLIP_XXX enumeration allows you to mirror the image
			SDL_RenderCopyEx(Game::renderer, font_texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

			//increment text_x!
			text_x += dest.w;
		}

		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}

