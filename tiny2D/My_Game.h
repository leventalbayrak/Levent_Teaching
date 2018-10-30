#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{
	//create this function
	void init_Maze(Grid::Grid *g);
	//create this function
	int next_Maze_Step(Grid::Point *current_pos, Grid::Grid *g, int &done);

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
			const int maze_n_rows = 80;
			const int maze_n_cols = 128;
			int step_frequency = 5;
			int hole_frequency = 5;
			
		}

		Tileset::Tileset tileset;
		Font::Font text;
		Particle::Emitter emitter;
		Particle::Emitter trace;
		Grid::Grid maze;
		Grid::Point current_maze_position = {0, 0};
		int steps[Parameters::maze_n_rows*Parameters::maze_n_cols];
		int n_steps = 0;
		
		Grid_Camera::Grid_Camera camera;
		
		unsigned int last_step_time = 0;
		unsigned int last_hole_time = 0;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Tileset::init(&World::tileset, "road_tileset.txt", Engine::renderer);
		Particle::init(&World::emitter, "dirt.txt", 512, Engine::renderer);
		Particle::init(&World::trace, "green.txt", 512, Engine::renderer);

		Grid::init(&World::maze, World::Parameters::maze_n_rows, World::Parameters::maze_n_cols);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::Parameters::maze_n_cols;
		World::camera.world_coord.h = World::Parameters::maze_n_rows;

	}

	void begin_Play(unsigned int current_time)
	{
		Font::set_Screen_Size(&World::text, 16, 16);

		init_Maze(&World::maze);

		World::current_maze_position.x = 1 + World::Parameters::maze_n_cols / 2;
		World::current_maze_position.y = 1 + World::Parameters::maze_n_rows / 2;
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	
		
		if (current_time - World::last_hole_time >= World::Parameters::hole_frequency)
		{
			World::last_hole_time = current_time;
			//int y = rand() % World::maze.n_rows;
			//int x = rand() % World::maze.n_cols;
			//World::maze.data[y*World::maze.n_cols + x] = 0;

		}

		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			int done = 0;
			int dir = next_Maze_Step(&World::current_maze_position, &World::maze, done);

			if (done == 1)
			{
				Grid::clear(&World::maze, 0);

				for (int i = 0; i < 100; i++)
				{
					int min_life = 500;
					int max_life = 1000;
					Vec2D::Vec2D vel = {};
					int y = rand() % World::maze.n_rows;
					int x = rand() % World::maze.n_cols;
					Vec2D::Vec2D pos = { x, y};
					Vec2D::Vec2D f_min = { -300,-300 };
					Vec2D::Vec2D f_max = { 300,300 };
					Particle::spawn(&World::emitter, 20, 0.5, &pos, &vel, &f_min, &f_max, min_life, max_life, current_time);
					Particle::spawn(&World::trace, 5, 0.5, &pos, &vel, &f_min, &f_max, min_life, max_life, current_time);
				}
			}

			Vec2D::Vec2D vel = {};
			Vec2D::Vec2D pos = { World::current_maze_position.x + 0.5,World::current_maze_position.y + 0.5 };
			Vec2D::Vec2D f_min = {-300,-300};
			Vec2D::Vec2D f_max = {300,300};
			if (dir == 0)
			{
				f_min.x = -200;
				f_max.x = 200;
				f_min.y = -200;
				f_max.y = 600;
			}
			else if (dir == 1)
			{
				f_min.x = -600;
				f_max.x = 200;
				f_min.y = -200;
				f_max.y = 200;
			}
			else if (dir == 2)
			{
				

				f_min.x = -200;
				f_max.x = 200;
				f_min.y = -600;
				f_max.y = 200;
			}
			else if (dir == 3)
			{

				f_min.x = -200;
				f_max.x = 600;
				f_min.y = -200;
				f_max.y = 200;

				
			}

			int min_life = 500;
			int max_life = 1000;
			Particle::spawn(&World::emitter, 20, 0.5, &pos, &vel, &f_min, &f_max, min_life, max_life, current_time);
			Particle::spawn(&World::trace, 5, 0.5, &pos, &vel, &f_min, &f_max, min_life, max_life, current_time);
		}

		Particle::update_Vel_and_Life(&World::trace, current_time, dt);
		Particle::update_Vel_and_Life(&World::emitter, current_time, dt);
		Particle::update_Pos(&World::trace, current_time, dt);
		Particle::update_Pos(&World::emitter, current_time, dt);
		
	}

	void draw(unsigned int current_time)
	{

		SDL_RenderClear(Engine::renderer);

		//World::camera.world_coord.x = World::current_maze_position.x;
		//World::camera.world_coord.y = World::current_maze_position.y;
		Grid_Camera::calibrate(&World::camera);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, Engine::renderer);
		Particle::draw(&World::emitter, &World::camera, current_time, Engine::renderer);
		Particle::draw(&World::trace, &World::camera, current_time, Engine::renderer);
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

	void init_Maze(Grid::Grid *g)
	{
		Grid::clear(g, 0);
		int r = 0;
		for (int y = 1; y < g->n_rows-1; y += 1)
		{
			for (int x = 1+r; x < g->n_cols-1; x += 2)
			{
				g->data[y*g->n_cols + x] = 0;
			}	

			r ^= 1;
		}
	}

	int next_Maze_Step(Grid::Point *current_pos, Grid::Grid *g, int &done)
	{
		done = 0;
		//printf("%d %d\n", current_pos->x, current_pos->y);
		int neighbors[4];
		int n_neighbors = 0;

		if (current_pos->y >= 3 && g->data[(current_pos->y-2)*g->n_cols + current_pos->x] == 0)
		{
			neighbors[n_neighbors++] = 0;
		}
		if (current_pos->x + 4 <= g->n_cols && (g->data[current_pos->y*g->n_cols + current_pos->x + 2] == 0))
		{
			neighbors[n_neighbors++] = 1;
		}
		if (current_pos->y + 4 <= g->n_rows && g->data[(current_pos->y + 2)*g->n_cols + current_pos->x] == 0)
		{
			neighbors[n_neighbors++] = 2;
		}
		if (current_pos->x >= 3 && g->data[current_pos->y*g->n_cols + current_pos->x - 2] == 0)
		{
			neighbors[n_neighbors++] = 3;
		}

		if (n_neighbors == 0)
		{
			if (World::n_steps == 0)
			{
				done = 1;
				return -1;
			}

			int k = World::steps[--World::n_steps];
			if (k == 0)
			{
				g->data[(current_pos->y) * g->n_cols + current_pos->x] = 2;
				g->data[(current_pos->y + 1) * g->n_cols + current_pos->x] = 2;

				current_pos->y += 2;
				
			}
			else if (k == 1)
			{
				g->data[current_pos->y * g->n_cols + current_pos->x] = 2;
				g->data[current_pos->y * g->n_cols + current_pos->x - 1] = 2;

				current_pos->x -= 2;
			}
			else if (k == 2)
			{
				g->data[(current_pos->y) * g->n_cols + current_pos->x] = 2;
				g->data[(current_pos->y - 1) * g->n_cols + current_pos->x] = 2;

				current_pos->y -= 2;
			}
			else if (k == 3)
			{
				g->data[current_pos->y * g->n_cols + current_pos->x] = 2;
				g->data[current_pos->y * g->n_cols + current_pos->x + 1] = 2;
				
				current_pos->x += 2;
			}

			
			return -1;
			//step back
			//return
		}

		int k = neighbors[rand() % n_neighbors];

		if (k == 0)
		{
			g->data[(current_pos->y - 2)*g->n_cols + current_pos->x] = 1;
			g->data[(current_pos->y - 1)*g->n_cols + current_pos->x] = 1;
			current_pos->y -= 2;

			World::steps[World::n_steps++] = 0;
		}
		else if (k == 1)
		{
			g->data[current_pos->y*g->n_cols + current_pos->x + 2] = 1;
			g->data[current_pos->y*g->n_cols + current_pos->x + 1] = 1;
			current_pos->x += 2;

			World::steps[World::n_steps++] = 1;
		}
		else if (k == 2)
		{
			g->data[(current_pos->y + 2)*g->n_cols + current_pos->x] = 1;
			g->data[(current_pos->y + 1)*g->n_cols + current_pos->x] = 1;
			current_pos->y += 2;

			World::steps[World::n_steps++] = 2;
		}
		else if (k == 3)
		{
			g->data[current_pos->y*g->n_cols + current_pos->x - 2] = 1;
			g->data[current_pos->y*g->n_cols + current_pos->x - 1] = 1;
			current_pos->x -= 2;

			World::steps[World::n_steps++] = 3;
		}

		return k;
	}
}