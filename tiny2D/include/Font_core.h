#pragma once
#include "Font_data.h"
#include "Tileset_core.h"
#include "Grid_Camera_core.h"
#include "SDL2-2.0.8/include/SDL.h"

namespace Font
{

	void init()
	{
		Tileset::init(&internal::tileset, 10);
	}
	/*
	the image file must be a 16 by 16 cell grid that contains 256 ascii characters in the order they appear in the ascii table.
	width and height of each cell must be passed using tile_w and tile_h
	you may leave the cells containing ascii characters that you will not be using blank
	fill the font image grid rows from left to right
	*/
	int add(const char *font_image_filename,int tile_w, int tile_h, SDL_Renderer *renderer)
	{
		int index = Tileset::make(&internal::tileset);
		Tileset::modify(index, &internal::tileset, font_image_filename, tile_w, tile_h, 16, 16, renderer);
		return index;
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
	void draw(Vec2D::Vec2D *text_pos, Vec2D::Vec2D *text_size, const char *str, int length, int font_tileset_index, const Grid_Camera::Grid_Camera *c, const SDL_Renderer *renderer)
	{
		float text_block_x = text_pos->x;
		float text_w = text_size->x;
		float text_h = text_size->y;
		for (int i = 0; i < length; i++)
		{
			if (str[i] == '\n')
			{
				text_pos->y += text_h;
				text_pos->x = text_block_x;
				continue;
			}

			int row = str[i] >> 4;//divide by 16
			int col = str[i] & 15;//str[i] % 16 but faster :)
			Tileset::draw(font_tileset_index, row, col, &internal::tileset, text_pos->x, text_pos->y, text_w, text_h, renderer);
			text_pos->x += text_w;

		}

	}

}