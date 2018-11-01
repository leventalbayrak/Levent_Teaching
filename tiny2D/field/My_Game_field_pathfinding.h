#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{

	void field_Make(float *field, Grid::Grid *g)
	{
		memset(field, 0, sizeof(float)*g->n_cols*g->n_rows);

		float _min = DBL_MAX;
		float _max = -DBL_MAX;
		for (int i = 0; i < g->n_rows; i++)
		{
			for (int j = 0; j < g->n_cols; j++)
			{
				
				if (g->data[i*g->n_cols + j] == 0)
				{
					float sign = -1.0;
					for (int y = 0; y < g->n_rows; y++)
					{
						for (int x = 0; x < g->n_cols; x++)
						{
							float dist = (i - y)*(i - y) + (j - x) * (j - x);
							if (dist == 0) continue;
							field[y*g->n_cols + x] += sign / sqrt(dist);
						}
					}
				}
				else if (g->data[i*g->n_cols + j] == 1)
				{

					field[i*g->n_cols + j] = 1;
				}
				else
				{
					//field[i*g->n_cols + j] = 0;
				}
			}
		}

		for (int i = 0; i < g->n_rows; i++)
		{
			for (int j = 0; j < g->n_cols; j++)
			{
				_min = min(_min, field[i*g->n_cols + j]);
				_max = max(_max, field[i*g->n_cols + j]);
			}
		}

		for (int i = 0; i < g->n_rows; i++)
		{
			for (int j = 0; j < g->n_cols; j++)
			{
				field[i*g->n_cols + j] -= _min;
				field[i*g->n_cols + j] /= (_max - _min);

				if (field[i*g->n_cols + j] < 0.0) field[i*g->n_cols + j] = 0;
				if (field[i*g->n_cols + j] > 1.0) field[i*g->n_cols + j] = 1;
			}
		}
	}

	void field_Move(int &x, int &y, float *field, int nrows, int ncols)
	{
		float best = 0;
		float dir = 0;

		float top = DBL_MAX;
		float right = DBL_MAX;
		float bottom = DBL_MAX;
		float left = DBL_MAX;

		if (y > 0) top = field[(y - 1)*ncols + x];
		if (x < ncols - 1) right = field[(y)*ncols + x + 1];
		if (y < nrows - 1)bottom = field[(y + 1)*ncols + x];
		if (x > 0)left = field[(y)*ncols + x - 1];

		if (top < right)
		{
			dir = 0;
			best = top;
		}
		else
		{
			dir = 1;
			best = right;
		}

		if (bottom < best)
		{
			dir = 2;
			best = bottom;
		}

		if (left < best)
		{
			dir = 3;
			best = left;
		}

		if (rand() % 5 == 0)
		{
			//dir = rand() % 4;
		}


		if (dir == 0)
		{
			y--;
		}
		else if (dir == 1)
		{
			x++;
		}
		else if (dir == 2)
		{
			y++;
		}
		else if (dir == 3)
		{
			x--;
		}
	}

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
			int step_frequency = 5;
			
		}

		Tileset::Tileset tileset;
		Font::Font text;

		Grid::Grid maze;

		Grid_Camera::Grid_Camera camera;
		
		unsigned int last_step_time = 0;
		float value_under_mouse = 0.0;

		Grid::Point current_position = { 5, 5 };
		float *field;
	}

	void init(int screen_w, int screen_h)
	{

		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::load(&World::maze, "path.csv");

		World::field = (float*)malloc(sizeof(float)*World::maze.n_rows*World::maze.n_cols);
		memset(World::field, 0, sizeof(float)*World::maze.n_rows*World::maze.n_cols);
		field_Make(World::field, &World::maze);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::maze.n_cols;
		World::camera.world_coord.h = World::maze.n_rows;
		
	}

	void begin_Play(unsigned int current_time)
	{
		Font::set_Screen_Size(&World::text, 16, 16);

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		Grid_Camera::calibrate(&World::camera);
	
		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			int done = 0;
		
			field_Move(World::current_position.x, World::current_position.y, World::field, World::maze.n_rows, World::maze.n_cols);

			if (World::maze.data[World::current_position.y*World::maze.n_cols + World::current_position.x] != 0)
			{
				World::maze.data[World::current_position.y*World::maze.n_cols + World::current_position.x] = 8;
			}
			
		}

		Vec2D::Vec2D screen_point = { Input::mouse_x ,Input::mouse_y };
		Vec2D::Vec2D grid_point;

		Grid_Camera::screen_to_Grid(&grid_point, &screen_point, &World::camera);

		{
			int x = grid_point.x;
			int y = grid_point.y;
			World::value_under_mouse = World::field[y * World::maze.n_cols + x];
		}

		if (Input::keys[SDL_SCANCODE_SPACE] == 1)
		{
			World::current_position.x = 1;
			World::current_position.y = 1;
		}

		if (Input::mouse_left == 1)
		{
			World::current_position.x = 1;
			World::current_position.y = 1;
			

			for (int i = 0; i < World::maze.n_rows; i++)
			{
				for (int j = 0; j < World::maze.n_cols; j++)
				{
					if (World::maze.data[i*World::maze.n_cols + j] == 8)
					{
						World::maze.data[i*World::maze.n_cols + j] = 6;
					}
				}
			}

			int x = grid_point.x;
			int y = grid_point.y;
			World::maze.data[y*World::maze.n_cols + x] = 1;
			field_Make(World::field, &World::maze);
		}

		if (Input::mouse_right == 1)
		{
			World::current_position.x = 1;
			World::current_position.y = 1;
		
			for (int i = 0; i < World::maze.n_rows; i++)
			{
				for (int j = 0; j < World::maze.n_cols; j++)
				{
					if (World::maze.data[i*World::maze.n_cols + j] == 8)
					{
						World::maze.data[i*World::maze.n_cols + j] = 6;
					}
				}
			}

			int x = grid_point.x;
			int y = grid_point.y;
			World::maze.data[y*World::maze.n_cols + x] = 6;
			field_Make(World::field, &World::maze);
		}
	}

	void draw(unsigned int current_time)
	{

		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, Engine::renderer);
		Grid::draw_Float(World::field, World::maze.n_rows, World::maze.n_cols, &World::camera, Engine::renderer);

		char str[256];
		sprintf(str, "%f", World::value_under_mouse);
		Font::set_Screen_Size(&World::text, 32, 32);
		Font::set_Screen_Pos(&World::text, 10, 10);
		Font::draw(&World::text, str, strlen(str), &World::camera, Engine::renderer);
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

	
}