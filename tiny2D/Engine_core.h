#pragma once
#include "Engine_data.h"
#include <iostream>
#include <assert.h>
#include <time.h>

#include "Tileset_core.h"
#include "Grid_core.h"
#include "Grid_Camera_core.h"
#include "Sprite_core.h"
#include "Body_core.h"
#include "Font_core.h"
#include "Audio_core.h"

namespace Engine
{
	void init(const char*window_title, int _screen_width, int _screen_height)
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
		srand(time(0));
		screen_width = _screen_width;
		screen_height = _screen_height;
		window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		Audio::init(2048);
		Font::init();
		Tileset::init(&tileset_db, 64);
		Sprite::init(&sprite_db, 64);
	}

	namespace E_Sprite
	{
		int add(const char *filename)
		{
			return Sprite::File::add(&sprite_db, filename, renderer);
		}

		int make_Instance(int which_sprite)
		{
			return Sprite::make_Instance(which_sprite, &sprite_db);
		}

		void modify(int which_sprite, int which_instance, int frame_duration)
		{
			Sprite::modify(which_sprite, which_instance, &sprite_db, frame_duration);
		}

		void draw(int which_sprite, int which_instance, Shape::Rect *world_coord, int dir, Grid_Camera::Calibration *c, Grid_Camera::Grid_Camera *cam)
		{
			Shape::Rect screen_rect;
			Grid_Camera::grid_to_Screen(&screen_rect, world_coord, c, cam);
			Sprite::draw(which_sprite, which_instance, &sprite_db, screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h, renderer, dir);
		}

		void update(int which_sprite, int which_instance, unsigned int current_time)
		{
			Sprite::update(which_sprite, which_instance, &sprite_db, current_time);
		}
	}
	namespace E_Tileset
	{
		void add(const char *filename)
		{
			Tileset::File::add(&tileset_db, filename, renderer);
		}

		void draw(int which_tileset, Grid_Camera::Calibration *c, const Grid::Region *grid_region, const Grid::Grid *g)
		{
			int ty = c->tile_y;
			for (int i = grid_region->y0; i <= grid_region->y1; i++)
			{
				int tx = c->tile_x;

				int *tmp_level_data = &g->data[i*g->n_cols];
				for (int j = grid_region->x0; j <= grid_region->x1; j++)
				{
					int grid_data = tmp_level_data[j];
					int tileset_idx = grid_data / tileset_db.n_cols[which_tileset];
					int tileset_offset = grid_data % tileset_db.n_cols[which_tileset];
					Tileset::draw(which_tileset, tileset_idx, tileset_offset, &tileset_db, tx, ty, c->tile_w, c->tile_h, renderer);

					tx += c->tile_w;
				}
				ty += c->tile_h;
			}

		}
	}
}