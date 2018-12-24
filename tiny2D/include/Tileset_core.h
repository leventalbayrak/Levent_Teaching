#pragma once
#pragma warning(disable:4996)
#include <assert.h>
#include "Tileset_data.h"
#include "RGBA_data.h"
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

namespace Tileset
{
	void init(Tileset *tileset, const char *filename, const SDL_Renderer *renderer);

	void draw_Single(int tile_row, int tile_col, const Tileset *t, int dest_x, int dest_y, int dest_w, int dest_h, const RGBA::RGBA *color, const SDL_Renderer *renderer);

	void draw_Grid(const Tileset *tileset, const Grid_Camera::Grid_Camera *cam, const Grid::Grid *g, const RGBA::RGBA *color, SDL_Renderer *renderer);


}

namespace Tileset
{
	void init(Tileset *tileset, const char *filename, const SDL_Renderer *renderer)
	{
		*tileset = {};

		char img_filename[256];
		FILE *f = fopen(filename, "r");
		int tile_w = 0;
		int tile_h = 0;
		int n_rows = 0;
		int n_cols = 0;
		int r = fscanf(f, "img=%s tile_w=%d tile_h=%d n_cols=%d n_rows=%d\n", img_filename, &tile_w, &tile_h, &n_cols, &n_rows);
		if (r != 5) return;
		
		SDL_Surface *sprite_surface = IMG_Load(img_filename);
		assert(sprite_surface);
		SDL_Texture *texture = SDL_CreateTextureFromSurface((SDL_Renderer*)renderer, sprite_surface);

		SDL_FreeSurface(sprite_surface);
		/*
		tile_w, tile_h = size of single tile in pixels
		n_cols, n_rows = number of rows and columns of tiles in the tileset file
		*/
		tileset->tile_w = tile_w;
		tileset->tile_h = tile_h;
		tileset->tex = texture;
		tileset->n_cols = n_cols;
		tileset->n_rows = n_rows;
	}

	void draw_Single(int tile_row, int tile_col, const Tileset *t, int dest_x, int dest_y, int dest_w, int dest_h, const RGBA::RGBA *color, const SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = tile_col * t->tile_w;
		src.y = tile_row * t->tile_h;
		src.w = t->tile_w;
		src.h = t->tile_h;

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_SetTextureAlphaMod(t->tex, color->a);
		SDL_SetTextureColorMod(t->tex, color->r, color->g, color->b);
		
		SDL_RenderCopyEx((SDL_Renderer*)renderer, t->tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}

	void draw_Grid(const Tileset *tileset, const Grid_Camera::Grid_Camera *cam, const Grid::Grid *g, const RGBA::RGBA *color, SDL_Renderer *renderer)
	{
		int x0 = cam->world_coord.x;
		int y0 = cam->world_coord.y;
		int x1 = cam->world_coord.x + cam->world_coord.w;
		int y1 = cam->world_coord.y + cam->world_coord.h;

		if (x0 < 0) x0 = 0;
		if (y0 < 0) y0 = 0;
		if (x1 >= g->n_cols - 1) x1 = g->n_cols - 1;
		if (y1 >= g->n_rows - 1) y1 = g->n_rows - 1;

		int ty = cam->calibration.tile_y;
		for (int i = y0; i <= y1; i++)
		{
			int tx = cam->calibration.tile_x;

			int *tmp_level_data = &g->data[i*g->n_cols];
			for (int j = x0; j <= x1; j++)
			{
				int grid_data = tmp_level_data[j];
				int tileset_row = grid_data / tileset->n_cols;
				int tileset_col = grid_data % tileset->n_cols;
				draw_Single(tileset_row, tileset_col, tileset, tx, ty, cam->calibration.tile_w, cam->calibration.tile_h, color, renderer);

				tx += cam->calibration.tile_w;
			}
			ty += cam->calibration.tile_h;
		}

	}

	
}
