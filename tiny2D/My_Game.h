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
			float separation_force_mag = 14.0/2;
			float cohesion_force_mag = 1.0 / 2;
			float alignment_force_mag = 1.0 / 2;
			float target_force_mag = 64 / 2;
			float obstacle_force_mag = 32.0 / 2;

			float obstacle_radius = 2.0;
			float separation_radius = 0.5;
			float cohesion_radius = 18.0;

			Vec2D::Vec2D spawn_point;
			Vec2D::Vec2D end_point;

			
		}
		
		Vec2D::Vec2D avg_pos;

		Tileset::Tileset tileset;
		Font::Font text;

		Actor::Factory boids;

		Grid::Grid map;
		Grid::Grid collision;
		Grid::Grid imprint;
		Grid::Grid obstacles;

		Grid_Camera::Grid_Camera camera;

		Vec2D::Vec2D mouse_grid_point;

		int n_obstacles = 1000;
		int n_boids = 1000;// 4096;

		unsigned char *neighbor_array = NULL;

		Actor::Factory indicator;

	}

	void init(int screen_w, int screen_h)
	{
		srand(time(0));
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);

		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::load(&World::map, "giant_map_map.csv");
		Grid::load(&World::collision, "giant_map_collision.csv");
		Grid::init(&World::imprint, World::map.n_rows, World::map.n_cols);
		Grid::init(&World::obstacles, World::map.n_rows, World::map.n_cols);

		Actor::init(&World::boids, World::n_boids);
		Actor::add_Animated_Sprite(&World::boids, "box.txt", Engine::renderer);

		Actor::init(&World::indicator, World::n_boids);
		Actor::add_Animated_Sprite(&World::indicator, "saitama_pink_run.txt", Engine::renderer);

		World::neighbor_array = (unsigned char*)malloc(sizeof(unsigned char) * World::n_boids);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);

	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 32;
		World::camera.world_coord.h = World::camera.world_coord.w * Engine::screen_height / Engine::screen_width;

		World::Parameters::spawn_point.x = 5.0;
		World::Parameters::spawn_point.y = 5.0;
		World::Parameters::end_point.x = World::map.n_cols - 5.0;
		World::Parameters::end_point.y = World::map.n_rows - 5.0;
	
		for (int i = 0; i < World::n_boids; i++)
		{
			float x = World::Parameters::spawn_point.x;
			float y = World::Parameters::spawn_point.y;

			int id = Actor::spawn(&World::boids, 0.25, current_time);
			Actor::set_Pos(id, x, y, &World::boids);

			Vec2D::Vec2D vel = { 1.0 - 2.0*rand() / RAND_MAX ,1.0 - 2.0*rand() / RAND_MAX };
			Actor::set_Vel(i, &vel, &World::boids);
		}

		for (int i = 0; i < World::obstacles.n_cols*World::obstacles.n_rows; i++)
		{
			World::obstacles.data[i] = -1;
		}

		for (int i = 0; i < World::n_obstacles; i++)
		{
			float x = 1.0 + (float)(World::map.n_cols-5.0)*rand() / RAND_MAX;
			float y = 1.0 + (float)(World::map.n_rows-5.0)*rand() / RAND_MAX;
			World::obstacles.data[(int)y*World::obstacles.n_cols + (int)x] = 1;
		}

		Actor::spawn(&World::indicator, 1.0, current_time);
	}

	void flock(float dt)
	{

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

			Vec2D::Vec2D random_disturbance;
			random_disturbance.x = 4.1*(1.0 - 2.0*rand() / RAND_MAX);
			random_disturbance.y = 4.1*(1.0 - 2.0*rand() / RAND_MAX);
			Actor::add_Force(i, &World::boids, &random_disturbance);

			Shape::Rect::Data *actor_world = Actor::get_World_Coord(i, &World::boids);

			Shape::Rect::Data cohesion_box;
			cohesion_box.w = 2.0*World::Parameters::cohesion_radius;
			cohesion_box.h = 2.0*World::Parameters::cohesion_radius;
			cohesion_box.x = actor_world->x - 0.5*cohesion_box.w;
			cohesion_box.y = actor_world->y - 0.5*cohesion_box.h;

			memset(World::neighbor_array, 0, sizeof(unsigned char)*World::boids.array_size);

			Grid::Region region;
			Grid::get_Region_Under_Shape(&region, &cohesion_box);
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
					if (World::neighbor_array[k] == 0)
					{
						World::neighbor_array[k] = 1;

						Vec2D::Vec2D *neighbor_pos = Actor::get_Pos(k, &World::boids);
						float distance = sqrt((neighbor_pos->x - actor_world->x)*(neighbor_pos->x - actor_world->x) + (neighbor_pos->y - actor_world->y)*(neighbor_pos->y - actor_world->y));

						if (distance <= World::Parameters::separation_radius)
						{
							Vec2D::Vec2D separation_force = { actor_world->x - neighbor_pos->x,actor_world->y - neighbor_pos->y };
							Vec2D::norm(&separation_force);
							Vec2D::scale(&separation_force, World::Parameters::separation_force_mag);
							
							Actor::add_Force(i, &World::boids, &separation_force);
						}

						if (distance <= World::Parameters::cohesion_radius)
						{
							avg_pos.x += neighbor_pos->x;
							avg_pos.y += neighbor_pos->y;

							Vec2D::Vec2D *neighbor_vel = Actor::get_Vel(k, &World::boids);
							avg_vel.x += neighbor_vel->x;
							avg_vel.y += neighbor_vel->y;

							n_neighbors++;
						}
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
					if (World::neighbor_array[k] == 0 && distance <= World::Parameters::separation_radius)
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

		}

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		

		flock(dt);

		//obstacles and targets
		for (int i = 0; i < World::boids.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::boids) == 0) continue;

			Vec2D::Vec2D *actor_pos = Actor::get_Pos(i, &World::boids);

			World::avg_pos.x += actor_pos->x;
			World::avg_pos.y += actor_pos->y;

			Vec2D::Vec2D target_force = { World::Parameters::end_point.x - actor_pos->x,World::Parameters::end_point.y - actor_pos->y };
			Vec2D::norm(&target_force);
			Vec2D::scale(&target_force, World::Parameters::target_force_mag);
			Actor::add_Force(i, &World::boids, &target_force);

			Grid::Region region;

			Shape::Rect::Data cohesion_box;
			cohesion_box.w = 2.0*World::Parameters::cohesion_radius;
			cohesion_box.h = 2.0*World::Parameters::cohesion_radius;
			cohesion_box.x = actor_pos->x - 0.5*cohesion_box.w;
			cohesion_box.y = actor_pos->y - 0.5*cohesion_box.h;

			Grid::get_Region_Under_Shape(&region, &cohesion_box);
			Grid::clip_Grid_Region(&region, World::obstacles.n_cols, World::obstacles.n_rows);
			for (int y = region.first_row; y <= region.last_row; y++)
			{
				for (int x = region.first_col; x <= region.last_col; x++)
				{
					int k = Grid::get_Tile(x, y, &World::obstacles);
					if (k == -1) continue;	

					
					Vec2D::Vec2D tile_pos = { x + 0.5, y + 0.5 };
					float distance = sqrt((tile_pos.x - actor_pos->x)*(tile_pos.x - actor_pos->x) + (tile_pos.y - actor_pos->y)*(tile_pos.y - actor_pos->y));
					
					if (distance <= World::Parameters::obstacle_radius)
					{
						Vec2D::Vec2D obs_force = { actor_pos->x - tile_pos.x, actor_pos->y - tile_pos.y };
						Vec2D::norm(&obs_force);
						Vec2D::scale(&obs_force, World::Parameters::obstacle_force_mag);
						Actor::add_Force(i, &World::boids, &obs_force);
					}
				}
			}
		}

		for (int i = 0; i < World::boids.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::boids) == 0) continue;

			Shape::Rect::Data *actor_world = Actor::get_World_Coord(i, &World::boids);

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

				Grid::Region region;
				Grid::get_Region_Under_Shape(&region, actor_world);
				Grid::clip_Grid_Region(&region, World::map.n_cols, World::map.n_rows);

				for (int y = region.first_row; y <= region.last_row; y++)
				{
					for (int x = region.first_col; x <= region.last_col; x++)
					{
						if (Grid::get_Tile(x, y, &World::obstacles) == -1) continue;
						Shape::Rect::Data tile_rect = { x,y,1.0,1.0 };
						Collision::impulse(actor_world, vel, 1.0, &tile_rect);
					}
				}

				Actor::update_Pos(i, &World::boids, dt2);
				if (actor_world->x < 1.0) actor_world->x = 1.0;
				if (actor_world->y < 1.0) actor_world->y = 1.0;
				if (actor_world->x + actor_world->w >= World::map.n_cols - 1) actor_world->x = World::map.n_cols - 1 - actor_world->w;
				if (actor_world->y + actor_world->h >= World::map.n_rows - 1) actor_world->y = World::map.n_rows - 1 - actor_world->h;
			}

			Actor::reset_Forces(i, &World::boids);
		}

		World::avg_pos.x /= World::boids.array_size;
		World::avg_pos.y /= World::boids.array_size;

		Actor::set_Pos(0, World::avg_pos.x, World::avg_pos.y, &World::indicator);

		World::camera.world_coord.x = World::avg_pos.x - World::camera.world_coord.w*0.5;
		World::camera.world_coord.y = World::avg_pos.y - World::camera.world_coord.h*0.5;

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
		Tileset::draw_Grid(&World::tileset, &World::camera, &World::obstacles, &RGBA::default, Engine::renderer);
		Actor::draw(&World::boids, &World::camera, current_time, Engine::renderer);
		Actor::draw(&World::indicator, &World::camera, current_time, Engine::renderer);
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}