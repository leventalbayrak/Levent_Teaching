#pragma once
#include "Font_data.h"
#include "Tileset_core.h"
#include "Grid_Camera_core.h"
#include "Shape_core.h"
#include "SDL2-2.0.8/include/SDL.h"

namespace Font
{
	/*
	the image file must be a 16 by 16 cell grid that contains 256 ascii characters in the order they appear in the ascii table.
	width and height of each cell must be passed using tile_w and tile_h
	you may leave the cells containing ascii characters that you will not be using blank
	fill the font image grid rows from left to right
	*/
	void init(Font *f, const char *font_image_filename, const SDL_Renderer *renderer)
	{
		Tileset::init(&f->tileset, font_image_filename, renderer);
		assert(f->tileset.n_rows == 16);
		assert(f->tileset.n_cols == 16);
	}

	void set_Screen_Pos(Font *f, float x, float y)
	{
		f->screen_coord.x = x;
		f->screen_coord.y = y;
	}

	void set_Screen_Size(Font *f, float w, float h)
	{
		f->screen_coord.w = w;
		f->screen_coord.h = h;
	}

	/*
	void draw(Vec2D::Vec2D *text_pos, const char *str, int length, int font_tileset_index, float size_in_tile_units, const Grid_Camera::Grid_Camera *c, const SDL_Renderer *renderer)
	Vec2D::Vec2D *text_pos = starting position of the text block, will be modified
	const char *str = string to print
	int length = length of the string
	int font_tileset_index = index of the tileset used for font
	float size_in_tile_units = 1.0 means that the characters will fit in 1 tile, 0.5 means half a tile
	const Grid_Camera::Grid_Camera *c = camera
	const SDL_Renderer *renderer = renderer
	*/
	void draw(Font *f, const char *str, int length, const Grid_Camera::Grid_Camera *c, const SDL_Renderer *renderer)
	{
		float text_block_x = f->screen_coord.x;
		float text_w = f->screen_coord.w;
		float text_h = f->screen_coord.h;
		for (int i = 0; i < length; i++)
		{
			if (str[i] == '\n')
			{
				f->screen_coord.y += text_h;
				f->screen_coord.x = text_block_x;
				continue;
			}

			int row = str[i] >> 4;//divide by 16
			int col = str[i] & 15;//str[i] % 16 but faster :)
			Tileset::draw_Single(row, col, &f->tileset, f->screen_coord.x, f->screen_coord.y, text_w, text_h, renderer);
			f->screen_coord.x += text_w;

		}

	}

}