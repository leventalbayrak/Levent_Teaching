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

		}

		Tileset::Tileset tileset;
		Font::Font text;

		Actor::Factory boids;

		Grid::Grid map;
		Grid::Grid collision;
		Grid::Grid imprint;

		Grid_Camera::Grid_Camera camera;

		Vec2D::Vec2D mouse_grid_point;

		int n_boids = 2000;
		int *neighbor_array = NULL;

	}

	void init(int screen_w, int screen_h)
	{
		srand(time(0));
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);

		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::load(&World::map, "sandbox_map.csv");
		Grid::load(&World::collision, "sandbox_collision.csv");
		Grid::init(&World::imprint, World::map.n_rows, World::map.n_cols);

		Actor::init(&World::boids, World::n_boids);
		Actor::add_Animated_Sprite(&World::boids, "box.txt", Engine::renderer);

		World::neighbor_array = (int*)malloc(sizeof(int) * World::n_boids);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);

	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 32;
		World::camera.world_coord.h = 32 * Engine::screen_height / Engine::screen_width;

		for (int i = 0; i < World::n_boids; i++)
		{
			float x = 10 + rand() % 3;// 1.0 + (float)(World::map.n_cols - 4.0)*rand() / RAND_MAX;
			float y = 10 + rand() % 3;// 1.0 + (float)(World::map.n_rows - 4.0)*rand() / RAND_MAX;

			int id = Actor::spawn(&World::boids, 0.5, current_time);
			Actor::set_Pos(id, x, y, &World::boids);
		}


	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();


		if (World::camera.world_coord.x < 0) World::camera.world_coord.x = 0;
		if (World::camera.world_coord.y < 0) World::camera.world_coord.y = 0;
		if (World::camera.world_coord.x >= World::map.n_cols - World::camera.world_coord.w) World::camera.world_coord.x = World::map.n_cols - World::camera.world_coord.w;
		if (World::camera.world_coord.y >= World::map.n_rows - World::camera.world_coord.h) World::camera.world_coord.y = World::map.n_rows - World::camera.world_coord.h;

		Grid_Camera::calibrate(&World::camera);
		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, Input::mouse_x, Input::mouse_y, &World::camera);

		for (int i = 0; i < World::imprint.n_cols*World::imprint.n_rows; i++)
		{
			World::imprint.data[i] = -1;
		}

		for (int i = 0; i < World::boids.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::boids) == 0) continue;

			Grid::imprint_Set(&World::imprint, i, Actor::get_World_Coord(i, &World::boids));

		}

		for (int i = 0; i < World::boids.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::boids) == 0) continue;

			Shape::Rect::Data *actor_world = Actor::get_World_Coord(i, &World::boids);
			
			

			Shape::Rect::Data radius;		
			radius.w = 4.0;
			radius.h = 4.0;
			radius.x = actor_world->x - 0.5*radius.w;
			radius.y = actor_world->y - 0.5*radius.h;

			memset(World::neighbor_array, 0, sizeof(int)*World::boids.array_size);
			Grid::Region region;
			Grid::get_Region_Under_Shape(&region, &radius);
			Grid::clip_Grid_Region(&region, World::map.n_cols, World::map.n_rows);
			for (int y = region.first_row; y <= region.last_row; y++)
			{
				for (int x = region.first_col; x <= region.last_col; x++)
				{
					int k = Grid::get_Tile(x, y, &World::imprint);
					if (k == -1) continue;
					if (k == i) continue;

					World::neighbor_array[k] = 1;
					
				}
			}


			int n_neighbors = 0;
			Vec2D::Vec2D avg_pos = { 0,0 };
			Vec2D::Vec2D avg_vel = { 0,0 };
			for (int j = 0; j < World::boids.array_size; j++)
			{
				if (World::neighbor_array[j] == 1)
				{
					//you found an agent within your radius
					Vec2D::Vec2D *neighbor_pos = Actor::get_Pos(j, &World::boids);
					avg_pos.x += neighbor_pos->x;
					avg_pos.y += neighbor_pos->y;


					Vec2D::Vec2D *neighbor_vel = Actor::get_Vel(j, &World::boids);
					avg_vel.x += neighbor_vel->x;
					avg_vel.y += neighbor_vel->y;

					n_neighbors++;

					Vec2D::Vec2D separation_force = { actor_world->x - neighbor_pos->x,actor_world->y - neighbor_pos->y };
					Vec2D::norm(&separation_force);
					Vec2D::scale(&separation_force, 10.0);

					Actor::add_Force(i, &World::boids, &separation_force);

				}
			}

			if (n_neighbors != 0)
			{
				avg_pos.x /= n_neighbors;
				avg_pos.y /= n_neighbors;
				avg_vel.x /= n_neighbors;
				avg_vel.y /= n_neighbors;


				Vec2D::Vec2D cohesion_force = { avg_pos.x - actor_world->x,avg_pos.y - actor_world->y };
				Vec2D::norm(&cohesion_force);
				Vec2D::scale(&cohesion_force, 20.0);

				Actor::add_Force(i, &World::boids, &cohesion_force);

				Vec2D::Vec2D alignment_force = { avg_vel.x, avg_vel.y };
				Vec2D::norm(&alignment_force);
				Vec2D::scale(&alignment_force, 10.0);

				Actor::add_Force(i, &World::boids, &alignment_force);
		

			}

			Vec2D::Vec2D target_force = { World::mouse_grid_point.x - actor_world->x,World::mouse_grid_point.y - actor_world->y };
			Vec2D::norm(&target_force);
			Vec2D::scale(&target_force, 20.0);

			Actor::add_Force(i, &World::boids, &target_force);

			//DONE ADDING FORCES
			//DO COLLISION

			Actor::update_Vel(i, &World::boids, dt);

			Actor::get_Vel(i, &World::boids)->x *= 0.95;
			Actor::get_Vel(i, &World::boids)->y *= 0.95;


			Grid::get_Region_Under_Shape(&region, actor_world);
			Grid::clip_Grid_Region(&region, World::map.n_cols, World::map.n_rows);
			for (int y = region.first_row; y <= region.last_row; y++)
			{
				for (int x = region.first_col; x <= region.last_col; x++)
				{
					if (Grid::get_Tile(x, y, &World::collision) == -1) continue;
					Shape::Rect::Data tile_rect = { x,y,1.0,1.0 };
					Collision::impulse(actor_world, Actor::get_Vel(i, &World::boids), 1.0, &tile_rect);
				}
			}
			Actor::update_Pos(i, &World::boids, dt);



		}



		

	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);

		Actor::draw(&World::boids, &World::camera, current_time, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}