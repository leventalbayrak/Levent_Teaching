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
			double separation_distance = 1.0;
			double cohesion_distance = 6.0;
			double alignment_distance = 4.0;

			double separation_force_mag = 1.0;
			double cohesion_force_mag = 1.0;
			double alignment_force_mag = 2.0;
			double target_force_mag = 12.0;

			double friction = 0.95;

			int n_boids = 1000;
		}


		Tileset::Tileset tileset;
		Font::Font text;
		
		Grid::Grid map;
		Grid::Grid collision;
		Grid::Grid imprint;

		Actor::Factory boids;
		
		Particle::Emitter trace;

		Grid_Camera::Grid_Camera camera;

		Vec2D::Vec2D mouse_grid_point;

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

		Actor::init(&World::boids, 4096);

		Actor::add_Animated_Sprite(&World::boids, "box.txt", Engine::renderer);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
	
		Particle::init(&World::trace, "dirt.txt", 4096, Engine::renderer);
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 32;
		World::camera.world_coord.h = 32 * Engine::screen_height/Engine::screen_width;

		for (int i = 0; i < World::Parameters::n_boids; i++)
		{
			double x = 1.0 + (World::map.n_cols - 2)*rand() / RAND_MAX;
			double y = 1.0 + (World::map.n_rows - 2)*rand() / RAND_MAX;
			int enemy_id = Actor::spawn(&World::boids, 0.25, current_time);
			Actor::set_Pos(enemy_id, x, y, &World::boids);
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
		
#pragma region FLOCKING
		{
			//stay away from others
			//stay close to center of mass
			//follow the flock
			//FLOCKING BEHAVIOR
			for (int i = 0; i < World::boids.array_size; i++)
			{
				if (Actor::is_Spawned(i, &World::boids) == 0) continue;//skip inactive

				Vec2D::Vec2D avg_pos = { 0,0 };
				Vec2D::Vec2D avg_vel = { 0,0 };
				int n_neighbors_cohesion = 0;
				int n_neighbors_alignment = 0;

				Vec2D::Vec2D *pos_i = Actor::get_Pos(i, &World::boids);

				for (int j = 0; j < World::boids.array_size; j++)
				{
					if (Actor::is_Spawned(j, &World::boids) == 0) continue;//skip inactive
					if (i == j) continue;//skip itself

					Vec2D::Vec2D *pos_j = Actor::get_Pos(j, &World::boids);

					double dx = (pos_i->x - pos_j->x)*(pos_i->x - pos_j->x);
					double dy = (pos_i->y - pos_j->y)*(pos_i->y - pos_j->y);
					double distance = sqrt(dx + dy);

					//push away from others
					if (distance <= World::Parameters::separation_distance)
					{
						Vec2D::Vec2D separation_force = { pos_i->x - pos_j->x, pos_i->y - pos_j->y };
						Vec2D::norm(&separation_force);

						separation_force.x *= World::Parameters::separation_force_mag;
						separation_force.y *= World::Parameters::separation_force_mag;

						Actor::add_Force(i, &World::boids, &separation_force);//APPLY FORCE
					}

					//sum pos of the neighbors
					if (distance <= World::Parameters::cohesion_distance)
					{
						avg_pos.x += pos_j->x;
						avg_pos.y += pos_j->y;
						n_neighbors_cohesion++;
					}

					//sum vel of the neighbors
					Vec2D::Vec2D *vel_j = Actor::get_Vel(j, &World::boids);
					if (distance <= World::Parameters::alignment_distance)
					{
						avg_vel.x += vel_j->x;
						avg_vel.y += vel_j->y;
						n_neighbors_alignment++;
					}
				}

				//calculate avg
				avg_pos.x /= n_neighbors_cohesion;
				avg_pos.y /= n_neighbors_cohesion;
				avg_vel.x /= n_neighbors_alignment;
				avg_vel.y /= n_neighbors_alignment;

				Vec2D::Vec2D cohesion_force = { avg_pos.x - pos_i->x, avg_pos.y - pos_i->y };
				Vec2D::norm(&cohesion_force);

				cohesion_force.x *= World::Parameters::cohesion_force_mag;
				cohesion_force.y *= World::Parameters::cohesion_force_mag;

				Actor::add_Force(i, &World::boids, &cohesion_force);//APPLY FORCE

				Vec2D::Vec2D alignment_force = avg_vel;
				Vec2D::norm(&alignment_force);

				alignment_force.x *= World::Parameters::alignment_force_mag;
				alignment_force.y *= World::Parameters::alignment_force_mag;

				Actor::add_Force(i, &World::boids, &alignment_force);//APPLY FORCE

			}
		}


		for (int i = 0; i < World::boids.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::boids) == 0) continue;//skip inactive
			
			Vec2D::Vec2D *pos_i = Actor::get_Pos(i, &World::boids);
			Vec2D::Vec2D target_force = { World::mouse_grid_point.x - pos_i->x,World::mouse_grid_point.y - pos_i->y };
			Vec2D::norm(&target_force);
			
			target_force.x *= World::Parameters::target_force_mag;
			target_force.y *= World::Parameters::target_force_mag;

			Actor::add_Force(i, &World::boids, &target_force);//APPLY FORCE
		}


		for (int i = 0; i < World::boids.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::boids) == 0) continue;//skip inactive

			Actor::update_Vel(i, &World::boids, dt);

			Shape::Rect::Data *world_coord = Actor::get_World_Coord(i, &World::boids);
			Grid::Region region;
			Grid::get_Region_Under_Shape(&region, world_coord);
			for (int y = region.first_row; y <= region.last_row; y++)
			{
				for (int x = region.first_col; x <= region.last_col; x++)
				{
					int k = Grid::get_Tile(x, y, &World::collision);
					
					if (k == -1) continue;

					Vec2D::Vec2D *boid_vel = Actor::get_Vel(i, &World::boids);
					Shape::Rect::Data tile_world = { x,y,1.0,1.0 };
					Collision::impulse(world_coord, boid_vel, 1.0, &tile_world);
				}
			}


			Actor::get_Vel(i, &World::boids)->x *= World::Parameters::friction;
			Actor::get_Vel(i, &World::boids)->y *= World::Parameters::friction;
			Actor::update_Pos(i, &World::boids, dt);

			Vec2D::Vec2D initial_vel = {};
			Vec2D::Vec2D f_min = { -50,-50 };
			Vec2D::Vec2D f_max = { 50,50 };
			Vec2D::Vec2D pos = { Actor::get_Pos(i,&World::boids)->x , Actor::get_Pos(i,&World::boids)->y};
			Particle::spawn(&World::trace, 1, 0.2, &pos, &initial_vel, &f_min, &f_max, 1000, 2000, current_time);

		}


		Particle::update_Vel_and_Life(&World::trace, current_time, dt);
		Particle::update_Pos(&World::trace, current_time, dt);
	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);
	
		Actor::draw(&World::boids, &World::camera, current_time, Engine::renderer);
		//Particle::draw(&World::trace, &World::camera, current_time, Engine::renderer);


		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}