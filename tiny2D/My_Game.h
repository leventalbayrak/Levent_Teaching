#pragma once

//BEAR RUN

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
			float bullet_force_magnitude = 500;
		}


		Tileset::Tileset tileset;
		Font::Font text;
		
		Grid::Grid map;
		Grid::Grid collision;

		Actor::Factory player;
		Actor::Factory enemy;
		Actor::Factory bullet;
		Particle::Emitter bullet_spark;

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

		Actor::init(&World::player, 2);
		Actor::init(&World::enemy, 512);
		Actor::init(&World::bullet, 1024);

		Actor::add_Animated_Sprite(&World::player, "saitama_pink_run.txt", Engine::renderer);
		Actor::add_Animated_Sprite(&World::enemy, "saitama_pink_run.txt", Engine::renderer);
		Actor::add_Animated_Sprite(&World::bullet, "box.txt", Engine::renderer);

		Particle::init(&World::bullet_spark, "box.txt", 1000, Engine::renderer);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
	
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 16;
		World::camera.world_coord.h = 16 * Engine::screen_height/Engine::screen_width;

		int player_id = Actor::spawn(&World::player, 1.0, current_time);
		
		Actor::set_Pos(player_id, 4, 4, &World::player);
		
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		Grid_Camera::calibrate(&World::camera);

		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, Input::mouse_x, Input::mouse_y, &World::camera);

		if (Input::mouse_left == 1 && Input::prev_mouse_left == 0)
		{
			int bullet_id = Actor::spawn(&World::bullet, 0.25, current_time);
			Vec2D::Vec2D *player_pos = Actor::get_Pos(0, &World::player);
			Actor::set_Pos(bullet_id, player_pos->x, player_pos->y, &World::bullet);

			Vec2D::Vec2D force = World::mouse_grid_point;
			Vec2D::sub(&force, Actor::get_Pos(bullet_id, &World::bullet));
			Vec2D::norm(&force);
			force.x *= World::Parameters::bullet_force_magnitude;
			force.y *= World::Parameters::bullet_force_magnitude;
			Actor::add_Force(bullet_id, &World::bullet, &force);
		}

		for (int i = 0; i < World::bullet.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::bullet) == 0) continue;
		
			if (current_time - World::bullet.creation_time[i] > 3000)
			{
				Actor::destroy(i, &World::bullet);
				continue;
			}

			Actor::update_Vel(i, &World::bullet, dt);
			Actor::update_Pos(i, &World::bullet, dt);

			Grid::Region region;
			Actor::get_Grid_Collision(&region, &World::collision, i, &World::bullet);
			
			bool done = false;

			for (int y = region.first_row; y <= region.last_row; y++)
			{
				for (int x = region.first_col; x <= region.last_col; x++)
				{
					if (Grid::get_Tile(x, y, &World::collision) == -1) continue;

					Vec2D::Vec2D vel = *(Actor::get_Vel(i, &World::bullet));
					vel.x *= -0.5;
					vel.y *= -0.5;
					Vec2D::Vec2D force_min = { -50,-50 };
					Vec2D::Vec2D force_max = { 50, 50 };
					Particle::spawn(&World::bullet_spark, 10, 0.1,
						Actor::get_Pos(i, &World::bullet), &vel,
						&force_min, &force_max, 500, 1000,
						current_time);


					Shape::Rect::Data tile_coord = { x,y,1,1 };
					int dir = Shape::Rect::collision_with_Dir(Actor::get_World_Coord(i, &World::bullet), &tile_coord);
					
					if (dir == 2 || dir == 4)
					{
						Actor::get_Vel(i, &World::bullet)->x *= -1;
					}
					if (dir == 1 || dir == 3)
					{
						Actor::get_Vel(i, &World::bullet)->y *= -1;
					}


					Actor::undo_Pos_Update(i, &World::bullet);

					done = true;
					break;
				}

				if (done) break;
			}

		}


		Particle::update_Vel_and_Life(&World::bullet_spark, current_time, dt);
		Particle::update_Pos(&World::bullet_spark, current_time, dt);

	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);

		Actor::draw(&World::player, &World::camera, current_time, Engine::renderer);
		Actor::draw(&World::bullet, &World::camera, current_time, Engine::renderer);

		Particle::draw(&World::bullet_spark, &World::camera, current_time, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}