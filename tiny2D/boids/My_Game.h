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
		
		Grid::Grid map;
		Grid::Grid collision;
		Actor::Factory boids;
		
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
	
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 32;
		World::camera.world_coord.h = 32 * Engine::screen_height/Engine::screen_width;

		
	
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