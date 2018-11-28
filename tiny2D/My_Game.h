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
			float separation_force_mag = 40.0;
			float cohesion_force_mag = 40.0;
			float alignment_force_mag = 20.0;
			float target_force_mag = 120.0;

			float radius = 2.0;
			float target_radius = 4.0;
		}

		Tileset::Tileset tileset;
		Font::Font text;

		Actor::Factory boids;

		Grid::Grid map;
		Grid::Grid collision;
		Grid::Grid imprint;

		Grid_Camera::Grid_Camera camera;

		Vec2D::Vec2D mouse_grid_point;

		int n_boids = 2000;// 4096;
		unsigned char *neighbor_array = NULL;

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

		World::neighbor_array = (unsigned char*)malloc(sizeof(unsigned char) * World::n_boids);

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
			float x = 2.0 + (float)(World::map.n_cols - 5.0)*rand() / RAND_MAX;
			float y = 2.0 + (float)(World::map.n_rows - 5.0)*rand() / RAND_MAX;

			int id = Actor::spawn(&World::boids, 0.25, current_time);
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
			radius.w = 2.0*World::Parameters::radius;
			radius.h = 2.0*World::Parameters::radius;
			radius.x = actor_world->x - 0.5*radius.w;
			radius.y = actor_world->y - 0.5*radius.h;

			memset(World::neighbor_array, 0, sizeof(unsigned char)*World::boids.array_size);

			Grid::Region region;
			Grid::get_Region_Under_Shape(&region, &radius);
			Grid::clip_Grid_Region(&region, World::map.n_cols, World::map.n_rows);

			int n_neighbors = 0;
			Vec2D::Vec2D avg_pos = { 0,0 };
			Vec2D::Vec2D avg_vel = { 0,0 };

			for (int y = region.first_row; y <= region.last_row; y++)
			{
				for (int x = region.first_col; x <= region.last_col; x++)
				{
					int k = Grid::get_Tile(x, y, &World::imprint);
					if (k == -1) continue;
					if (k == i) continue;

					Vec2D::Vec2D *neighbor_pos = Actor::get_Pos(k, &World::boids);
					float distance = sqrt((neighbor_pos->x - actor_world->x)*(neighbor_pos->x - actor_world->x) + (neighbor_pos->y - actor_world->y)*(neighbor_pos->y - actor_world->y));

					//you found an agent within your radius
					if (World::neighbor_array[k] == 0 && distance <= World::Parameters::radius)
					{
						World::neighbor_array[k] = 1;
						
						avg_pos.x += neighbor_pos->x;
						avg_pos.y += neighbor_pos->y;


						Vec2D::Vec2D *neighbor_vel = Actor::get_Vel(k, &World::boids);
						avg_vel.x += neighbor_vel->x;
						avg_vel.y += neighbor_vel->y;

						n_neighbors++;

						Vec2D::Vec2D separation_force = { actor_world->x - neighbor_pos->x,actor_world->y - neighbor_pos->y };
						Vec2D::norm(&separation_force);
						Vec2D::scale(&separation_force, World::Parameters::separation_force_mag);

						Actor::add_Force(i, &World::boids, &separation_force);
					}
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
				Vec2D::scale(&cohesion_force, World::Parameters::cohesion_force_mag);

				Actor::add_Force(i, &World::boids, &cohesion_force);

				Vec2D::Vec2D alignment_force = { avg_vel.x, avg_vel.y };
				Vec2D::norm(&alignment_force);
				Vec2D::scale(&alignment_force, World::Parameters::alignment_force_mag);

				Actor::add_Force(i, &World::boids, &alignment_force);
			}

	
			float distance = sqrt((World::mouse_grid_point.x - actor_world->x)*(World::mouse_grid_point.x - actor_world->x) + (World::mouse_grid_point.y - actor_world->y)*(World::mouse_grid_point.y - actor_world->y));
			if (distance <= World::Parameters::target_radius)
			{
				Vec2D::Vec2D target_force = { World::mouse_grid_point.x - actor_world->x,World::mouse_grid_point.y - actor_world->y };
				Vec2D::norm(&target_force);
				Vec2D::scale(&target_force, World::Parameters::target_force_mag);

				Actor::add_Force(i, &World::boids, &target_force);
			}
			//DONE ADDING FORCES
			//DO COLLISION
			int n_steps = 5;
			for (int j = 0; j < n_steps; j++)
			{
				float dt2 = dt / n_steps;
				Actor::update_Vel(i, &World::boids, dt2);

				Vec2D::Vec2D *vel = Actor::get_Vel(i, &World::boids);
				vel->x *= 0.95;
				vel->y *= 0.95;


				Grid::get_Region_Under_Shape(&region, actor_world);
				Grid::clip_Grid_Region(&region, World::map.n_cols, World::map.n_rows);
				for (int y = region.first_row; y <= region.last_row; y++)
				{
					for (int x = region.first_col; x <= region.last_col; x++)
					{
						if (Grid::get_Tile(x, y, &World::collision) == -1) continue;
						Shape::Rect::Data tile_rect = { x,y,1.0,1.0 };
						Collision::impulse(actor_world, vel, 1.0, &tile_rect);
					}
				}

				Actor::update_Pos(i, &World::boids, dt2);
				if (actor_world->x < 1.0) actor_world->x = 1.0;
				if (actor_world->y < 1.0) actor_world->y = 1.0;
				if (actor_world->x + actor_world->w >= World::map.n_cols-1) actor_world->x = World::map.n_cols - 1 - actor_world->w;
				if (actor_world->y + actor_world->h >= World::map.n_rows-1) actor_world->y = World::map.n_rows - 1 - actor_world->h;
			}

			Actor::reset_Forces(i, &World::boids);
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