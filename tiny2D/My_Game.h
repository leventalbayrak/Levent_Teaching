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

		}

		Tileset::Tileset tileset;

		Particle::Emitter emitter;

		Grid_Camera::Grid_Camera camera;
		Grid::Grid tile_map;
		Grid::Grid background_map;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Tileset::init(&World::tileset,16);
		Tileset::File::add(&World::tileset, "map_tileset.txt", Engine::renderer);

		Particle::init(&World::emitter, "saitama_pink_run.txt", 256, Engine::renderer);

		Grid::load(&World::tile_map, "bear_run_tilemap_tilemap.csv");
		Grid::load(&World::background_map, "bear_run_tilemap_background.csv");

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 1;
		World::camera.world_coord.w = 0.025 * Engine::screen_width;
		World::camera.world_coord.h = 0.025 * Engine::screen_height;

	}

	void begin_Play()
	{
		
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	
		//add forces to actors

		
		//after done adding all forces
		//execute the loop below

		Vec2D::Vec2D mouse_pos_screen = { Engine::Input::mouse_x-32, Engine::Input::mouse_y-32};
		Vec2D::Vec2D mouse_pos_grid;
		Grid_Camera::screen_to_Grid(&mouse_pos_grid, &mouse_pos_screen, &World::camera);

		World::emitter.emitter_world_coords = mouse_pos_grid;

		Vec2D::Vec2D f_min, f_max;
		f_min.x = -80;
		f_max.x = 80;
		f_min.y = -400;
		f_max.y = -420;
		Particle::spawn(&World::emitter, 1, &mouse_pos_grid, &f_min, &f_max, 2000, 5000, current_time);

		Vec2D::Vec2D gravity = { 0, 5 };
		Particle::apply_Force(&World::emitter, &gravity);

		Particle::update_Vel_and_Life(&World::emitter, current_time, dt);
		Particle::update_Pos(&World::emitter, current_time, dt);
		
	}

	void draw(unsigned int current_time)
	{

		
		SDL_RenderClear(Engine::renderer);

		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 1;
		Grid_Camera::calibrate(&World::camera);

		Tileset::draw_Grid(0, &World::tileset, &World::camera, &World::background_map, Engine::renderer);
		Tileset::draw_Grid(0, &World::tileset, &World::camera, &World::tile_map, Engine::renderer);
		
		Particle::draw(&World::emitter, &World::camera, current_time, Engine::renderer);
		int active_count = 0;
		for (int i = 0; i < World::emitter.array_size; i++)
		{
			if (World::emitter.state[i] == 1) active_count++;
		}
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}
}