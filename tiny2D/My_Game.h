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
			int step_frequency = 0;
			int n_points = 20;
			float box_size = 1.0;
			float permute_amount = 24.0;//<-- VERY IMPORTANT RANK 3
			float temperature_decay = 0.98;//<-- VERY IMPORTANT RANK 1
			int n_permute_same_temp = 80;//<-- VERY IMPORTANT RANK 2
		}

		Tileset::Tileset tileset;
		Font::Font text;

		Grid::Grid maze;

		Grid_Camera::Grid_Camera camera;

		unsigned int last_step_time = 0;


		Vec2D::Vec2D mouse_grid_point;

		float temperature = 1.0;

		float fitness = 0.0;

		Shape::Rect::Factory points;
		Shape::Rect::Factory tmp_points;
	}

	void init(int screen_w, int screen_h)
	{
		srand(time(0));
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Shape::Rect::init(&World::points, World::Parameters::n_points);
		Shape::Rect::init(&World::tmp_points, World::Parameters::n_points);

		Grid::load(&World::maze, "collision.csv");

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::maze.n_cols;
		World::camera.world_coord.h = World::maze.n_rows;

	}

	void begin_Play(unsigned int current_time)
	{
		RGBA::RGBA text_color = { 0,0,0,255 };
		Font::set_Screen_Size(&World::text, 16, 16);
		Font::set_RGBA(&World::text, &text_color);

		for (int i = 0; i < World::Parameters::n_points; i++)
		{
			Shape::Rect::make_Instance(&World::points);
			Shape::Rect::make_Instance(&World::tmp_points);
		}

		
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		Grid_Camera::calibrate(&World::camera);


		Vec2D::Vec2D screen_point = { Input::mouse_x ,Input::mouse_y };
		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, &screen_point, &World::camera);


		

		//
	}

	void draw(unsigned int current_time)
	{
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		SDL_RenderClear(Engine::renderer);

		RGBA::RGBA tile_tint = { 255,255,255,255 };
		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, &tile_tint, Engine::renderer);

		RGB::RGB color = { 255,0,0 };
		Shape::Rect::draw(&World::points, &color, &World::camera, Engine::renderer);
		Shape::Rect::draw_Connecting_Lines(&World::points, &color, &World::camera, Engine::renderer);

		char str[256];
		sprintf(str, "temperature %f\nfitness %f", World::temperature, World::fitness);
		Font::set_Screen_Size(&World::text, 32, 32);
		Font::set_Screen_Pos(&World::text, 10, 10);
		Font::draw(&World::text, str, strlen(str), &World::camera, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}


}