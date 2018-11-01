#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{

	namespace Command
	{

	}

	namespace Assets
	{
		namespace Animation
		{
			
		}
	}

	namespace World
	{
		
		namespace Parameters
		{
			int step_frequency = 0;
			int n_nodes = 512;
			
		}

		Tileset::Tileset tileset;
		Font::Font text;

		Grid::Grid maze;

		Grid_Camera::Grid_Camera camera;
		
		unsigned int last_step_time = 0;

		Actor::Factory nodes;
		Vec2D::Vec2D mouse_grid_point;
		float move_amount = 10;
		float move_decay = 0.95;
	}

	void init(int screen_w, int screen_h)
	{

		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Actor::init(&World::nodes, 1024);
		Actor::add_Sprite(&World::nodes, "ball.txt", Engine::renderer);

		Grid::load(&World::maze, "path.csv");

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::maze.n_cols;
		World::camera.world_coord.h = World::maze.n_rows;
		
	}

	void begin_Play(unsigned int current_time)
	{
		Font::set_Screen_Size(&World::text, 16, 16);

		//World::Parameters::n_nodes = World::maze.n_cols;

		for (int i = 0; i < World::Parameters::n_nodes; i++)
		{
			int k = Actor::spawn(&World::nodes, 0.5, current_time);
			float x = World::maze.n_cols * (double)rand() / RAND_MAX;
			float y = World::maze.n_rows * (double)rand() / RAND_MAX;
			Actor::set_Pos(k, x, y, &World::nodes);
		}
		Actor::set_Pos(0, 1, World::maze.n_rows/2, &World::nodes);
		Actor::set_Pos(World::Parameters::n_nodes-1, World::maze.n_cols-2, World::maze.n_rows / 2, &World::nodes);

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		Grid_Camera::calibrate(&World::camera);
	
		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			
			for (int i = 1; i < World::Parameters::n_nodes - 1; i++)
			{
				
				float new_x = 0.5*(Actor::get_Pos(i - 1, &World::nodes)->x + Actor::get_Pos(i + 1, &World::nodes)->x);
				float new_y = 0.5*(Actor::get_Pos(i - 1, &World::nodes)->y + Actor::get_Pos(i + 1, &World::nodes)->y);
				new_x += World::move_amount*(1.0 - 2.0*rand() / RAND_MAX);
				new_y += World::move_amount*(1.0 - 2.0*rand() / RAND_MAX);

				if (new_x < 0) new_x = 0;
				if (new_x > World::maze.n_cols - 1) new_x = World::maze.n_cols - 1;

				if (new_y < 0) new_y = 0;
				if (new_y > World::maze.n_rows - 1) new_y = World::maze.n_rows - 1;

				Shape::Rect::Data rect = { new_x, new_y, Actor::get_World_Coord(i, &World::nodes)->w,Actor::get_World_Coord(i, &World::nodes)->h };
				Grid::Region region;
				Grid::get_Region_Under_Shape(&region, &rect);

				int collision = 0;
				for (int y = region.y0; y <= region.y1; y++)
				{
					for (int x = region.x0; x <= region.x1; x++)
					{
						if (World::maze.data[y*World::maze.n_cols + x] == 1)
						{
							collision = 1;
							break;
						}
					}

					if (collision == 1) break;
				}

				if (collision == 0)
				{
					Actor::set_Pos(i, new_x, new_y, &World::nodes);
				}
				
			}
			
			World::move_amount *= World::move_decay;
		}

		Vec2D::Vec2D screen_point = { Input::mouse_x ,Input::mouse_y };
		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, &screen_point, &World::camera);


	}

	void draw(unsigned int current_time)
	{

		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, Engine::renderer);

		Actor::draw(&World::nodes, &World::camera, current_time, Engine::renderer);
		
		char str[256];
		sprintf(str, "%f", World::move_decay);
		Font::set_Screen_Size(&World::text, 32, 32);
		Font::set_Screen_Pos(&World::text, 10, 10);
		Font::draw(&World::text, str, strlen(str), &World::camera, Engine::renderer);
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

	
}