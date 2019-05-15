#pragma once

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{

	namespace World
	{

		namespace Parameters
		{
			Vec2D::Vec2D gravity = { 0,4 };
			Vec2D::Vec2D spawn_point = {10,10 };
		}
		

		Tileset::Tileset tileset;
		Font::Font text;

		Actor::Factory balls;

		Grid::Grid map;
		Grid::Grid collision;
		Grid::Grid imprint;

		Grid_Camera::Grid_Camera camera;

		Vec2D::Vec2D mouse_grid_point;

		int n_balls = 4000;// 4096;

	}

	void init(int screen_w, int screen_h)
	{
		srand(time(0));
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::load(&World::map, "testbox_map.csv");
		Grid::load(&World::collision, "testbox_collision.csv");
		Grid::init(&World::imprint, World::map.n_rows, World::map.n_cols);

		Actor::init(&World::balls, World::n_balls);
		Actor::add_Animated_Sprite(&World::balls, "box.txt", Engine::renderer);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);

	}

	void begin_Play(unsigned int current_time)
	{

		World::camera.world_coord.w = 40;
		World::camera.world_coord.h = World::camera.world_coord.w * Engine::screen_height / Engine::screen_width;
		World::camera.world_coord.x = World::Parameters::spawn_point.x - 0.5*World::camera.world_coord.w;
		World::camera.world_coord.y = World::Parameters::spawn_point.y - 0.5*World::camera.world_coord.h;
		Grid_Camera::calibrate(&World::camera);

		for (int i = 0; i < World::n_balls; i++)
		{
			float x = World::Parameters::spawn_point.x;
			float y = World::Parameters::spawn_point.y;

			int id = Actor::spawn(&World::balls, 0.25, current_time);
			Actor::set_Pos(id, x, y, &World::balls);

			Vec2D::Vec2D vel = { 8.0*(1.0 - 2.0*rand() / RAND_MAX) , 1.0*(1.0 - 2.0*rand() / RAND_MAX) };
			Actor::set_Vel(i, &vel, &World::balls);
		}

	}

	void update(unsigned int current_time, float dt)
	{
		Engine::event_Loop();

		
		//for (int i = 0; i < World::imprint.n_cols*World::imprint.n_rows; i++)
		//{
		//	World::imprint.data[i] = -1;
		//}

		////cant handle multiple inmprints in 1 cell
		//for (int i = 0; i < World::balls.array_size; i++)
		//{
		//	Shape::Rect::Data *actor_world = Actor::get_World_Coord(i, &World::balls);
		//	Grid::imprint_Set(&World::imprint, 1, actor_world);
		//}

		for (int i = 0; i < World::balls.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::balls) == 0) continue;

			Shape::Rect::Data *actor_world = Actor::get_World_Coord(i, &World::balls);

			//DONE ADDING FORCES
			//DO COLLISION
			int n_steps = 5;
			for (int j = 0; j < n_steps; j++)
			{
				float dt2 = dt / n_steps;
				Actor::update_Vel(i, &World::balls, dt2);

				Vec2D::Vec2D *vel = Actor::get_Vel(i, &World::balls);
				vel->x *= 0.999;
				vel->y *= 0.999;

				Grid::Region region;
				Grid::get_Region_Under_Shape(&region, actor_world);
				Grid::clip_Grid_Region(&region, World::map.n_cols, World::map.n_rows);
			
				Actor::add_Force(i, &World::balls, &World::Parameters::gravity);

				for (int y = region.first_row; y <= region.last_row; y++)
				{
					for (int x = region.first_col; x <= region.last_col; x++)
					{
						if (Grid::get_Tile(x, y, &World::collision) == -1) continue;
						Shape::Rect::Data tile_rect = { x,y,1,1 };
						Collision::impulse(actor_world, vel, 1.0, &tile_rect);
					}
				}

				/*for (int y = region.first_row; y <= region.last_row; y++)
				{
					for (int x = region.first_col; x <= region.last_col; x++)
					{
						if (Grid::get_Tile(x, y, &World::imprint) == -1) continue;
						Shape::Rect::Data tile_rect = { x,y,0.1,0.1 };
						Collision::impulse(actor_world, vel, 1.0, &tile_rect);
					}
				}*/

				Actor::update_Pos(i, &World::balls, dt2);
				if (actor_world->x < 1.0) actor_world->x = 1.0;
				if (actor_world->y < 1.0) actor_world->y = 1.0;
				if (actor_world->x + actor_world->w >= World::map.n_cols - 1) actor_world->x = World::map.n_cols - 1 - actor_world->w;
				if (actor_world->y + actor_world->h >= World::map.n_rows - 1) actor_world->y = World::map.n_rows - 1 - actor_world->h;
			}

			Actor::reset_Forces(i, &World::balls);
		}

		if (World::camera.world_coord.x < 0) World::camera.world_coord.x = 0;
		if (World::camera.world_coord.y < 0) World::camera.world_coord.y = 0;
		if (World::camera.world_coord.x >= World::map.n_cols - World::camera.world_coord.w) World::camera.world_coord.x = World::map.n_cols - World::camera.world_coord.w;
		if (World::camera.world_coord.y >= World::map.n_rows - World::camera.world_coord.h) World::camera.world_coord.y = World::map.n_rows - World::camera.world_coord.h;

		Grid_Camera::calibrate(&World::camera);
	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);
		Actor::draw(&World::balls, &World::camera, current_time, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}