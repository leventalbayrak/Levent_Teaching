#pragma once
#include <assert.h>
#include "Tilemap_data.h"
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

namespace Tileset
{

	void load(Tileset *t, char *image_filename, int tile_w, int tile_h, SDL_Renderer *renderer)
	{
		SDL_Surface *sprite_surface = IMG_Load(image_filename);
		assert(sprite_surface);
		SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);

		SDL_FreeSurface(sprite_surface);

		t->tile_w = tile_w;
		t->tile_h = tile_h;
		t->tex = sprite_texture;
	}

	void draw_with_Alpha(int dest_x, int dest_y, int dest_w, int dest_h, float alpha, int tileset_row, int tileset_col, Tileset *t, SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = tileset_col * t->tile_w;
		src.y = tileset_row * t->tile_h;
		src.w = t->tile_w;
		src.h = t->tile_h;

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_SetTextureAlphaMod(t->tex, alpha*255.0);
		SDL_RenderCopyEx(renderer, t->tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void draw(int dest_x, int dest_y, int dest_w, int dest_h, int tileset_row, int tileset_col, Tileset *t, SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = tileset_col * t->tile_w;
		src.y = tileset_row * t->tile_h;
		src.w = t->tile_w;
		src.h = t->tile_h;

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_RenderCopyEx(renderer, t->tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}
}
