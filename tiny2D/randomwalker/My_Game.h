#pragma once

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{
	int m(int x, int y, int ncols)
	{
		return y*ncols + x;
	}

	void populate_Map_Example(Grid::Grid *g)
	{
		for (int y = 0; y < g->n_rows; y++)
		{
			for (int x = 0; x < g->n_cols; x++)
			{
				g->data[m(x, y, g->n_cols)] = rand() % 3;
			}
		}
	}

	void random_Walk_Step(Grid::Point *p, Grid::Grid *g)
	{
		switch (rand() % 4)
		{


		}

		g->data[m(p->x, p->y, g->n_cols)] = 1;

	}

	namespace World
	{
		//manages a tileset image
		Tileset::Tileset tileset;

		//manages font
		Font::Font text;
		
		Grid::Point current_position;
		Grid::Grid map;

		Grid_Camera::Grid_Camera camera;

	}

	//do allocations and system initialization here
	void init(int screen_w, int screen_h)
	{
		srand(time(0));

		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::init(&World::map, 48, 64);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
	
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::map.n_cols;
		World::camera.world_coord.h = World::map.n_rows;
		
		memset(World::map.data, 0, sizeof(int)*World::map.n_cols*World::map.n_rows);

		World::current_position.x = World::map.n_cols / 2;
		World::current_position.y = World::map.n_rows / 2;
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
		Grid_Camera::calibrate(&World::camera);

		random_Walk_Step(&World::current_position, &World::map);

	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}