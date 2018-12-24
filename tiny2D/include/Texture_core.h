#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "SDL2-2.0.8/include/SDL_image.h"

namespace Texture
{
	SDL_Texture *load(const char *filename, const SDL_Renderer *renderer);
}

namespace Texture
{
	SDL_Texture *load(const char *filename,const SDL_Renderer *renderer)
	{
		SDL_Surface *sprite_surface = IMG_Load(filename);
		SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface((SDL_Renderer*)renderer, sprite_surface);
		SDL_FreeSurface(sprite_surface);
		return sprite_texture;
	}
}