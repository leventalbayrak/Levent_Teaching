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
		Actor::add_Animated_Sprite(&World::bullet, "saitama_pink_run.txt", Engine::renderer);

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
			int bullet_id = Actor::spawn(&World::bullet, 1.0, current_time);
			Vec2D::Vec2D *player_pos = Actor::get_Pos(0, &World::player);
			Actor::set_Pos(bullet_id, player_pos->x, player_pos->y, &World::bullet);

			Vec2D::Vec2D force = World::mouse_grid_point;
			Vec2D::sub(&force, Actor::get_Pos(bullet_id, &World::bullet));
			Vec2D::norm(&force);
			force.x *= World::Parameters::bullet_force_magnitude;
			force.y *= World::Parameters::bullet_force_magnitude;
			Actor::add_Force(bullet_id, &World::bullet, &force);
		}

		Actor::update_Vel(&World::bullet, dt);
		Actor::update_Pos(&World::bullet, dt);



	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);

		Actor::draw(&World::player, &World::camera, current_time, Engine::renderer);
		Actor::draw(&World::bullet, &World::camera, current_time, Engine::renderer);
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}