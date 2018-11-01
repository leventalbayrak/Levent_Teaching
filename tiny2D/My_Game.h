#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"



namespace My_Game
{

	void generate_New_Solution(Shape::Rect::Data *shapes, int n_shapes,float w, float h, float max_x, float max_y)
	{
		shapes[0] = { (float)1.0,(float)(max_y / 2.0),w,h };
		shapes[n_shapes - 1] = { (float)(max_x - 2.0),(float)(max_y / 2.0),w,h };
		for (int i = 1; i < n_shapes - 1; i++)
		{
			float x = max_x * (double)rand() / RAND_MAX;
			float y = max_y * (double)rand() / RAND_MAX;
			
			shapes[i].x = x;
			shapes[i].y = y;
			shapes[i].w = w;
			shapes[i].h = h;
		}
	}

	double _calculate_Solution_Fitness(Shape::Rect::Data *shapes, int n_shapes, Grid::Grid *collision)
	{
		double fitness = 0.0;
		
		for (int i = 1; i < n_shapes-1; i++)
		{
			int k = Grid::get_First_Overlapped_Tile(-1, &shapes[i], collision);
			if (k != -1)
			{
				fitness -= 20;
			}

			double dist_from_neighbor_mid = 0.0;
			double x = 0.5*(shapes[i - 1].x + shapes[i + 1].x);
			double y = 0.5*(shapes[i - 1].y + shapes[i + 1].y);
			dist_from_neighbor_mid += (shapes[i].x - x)*(shapes[i].x - x);
			dist_from_neighbor_mid += (shapes[i].y - y)*(shapes[i].y - y);
			
			fitness -= 2.0*sqrt(dist_from_neighbor_mid);

			
		}
		float distance_sum = 0.0;
		for (int i = 1; i < n_shapes; i++)
		{
			float x = (shapes[i - 1].x - shapes[i].x)*(shapes[i - 1].x - shapes[i].x);
			float y = (shapes[i - 1].y - shapes[i].y)*(shapes[i - 1].y - shapes[i].y);

			distance_sum += sqrt(x + y);

		}
		fitness -= 0.9*distance_sum;

		fitness /= n_shapes;
		return fitness;
	}

	void _permute_Solution(Shape::Rect::Data *dest, Shape::Rect::Data *src, int n_shapes, float amount, float max_x, float max_y)
	{
		memcpy(dest, src, sizeof(Shape::Rect::Data)*n_shapes);
		int n_permute = 1;
		//for (int i = 1; i < n_shapes-1; i++)
		for (int i = 0; i < n_permute; i++)
		{
			int k = 1 + rand() % (n_shapes-2);
			float x_amount = amount * (1.0 - 2.0*rand() / RAND_MAX);
			float y_amount = amount * (1.0 - 2.0*rand() / RAND_MAX);
	
			dest[k].x += x_amount;
			dest[k].y += y_amount;
			
			if (dest[k].x < 0) dest[k].x = 0;
			if (dest[k].y < 0) dest[k].y = 0;
			if (dest[k].x > max_x) dest[k].x = max_x;
			if (dest[k].y > max_y) dest[k].y = max_y;
		}
	}

	float update_Solution(Shape::Rect::Data *src, Shape::Rect::Data *tmp, int n_shapes, Grid::Grid *collision, float permute_amount, float temperature)
	{

		_permute_Solution(tmp, src, n_shapes, permute_amount, collision->n_cols-1,collision->n_rows-1);
		
		float current_fitness = _calculate_Solution_Fitness(src, n_shapes, collision);
		float permute_fitness = _calculate_Solution_Fitness(tmp, n_shapes, collision);

		float p = exp((permute_fitness - current_fitness) / temperature);
		float r = (double)rand() / RAND_MAX;
		if (r < p)
		{
			memcpy(src, tmp, sizeof(Shape::Rect::Data)*n_shapes);
			return permute_fitness;
		}

		return current_fitness;
	}

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
			int n_points = 24;
		}

		Tileset::Tileset tileset;
		Font::Font text;

		Grid::Grid maze;

		Grid_Camera::Grid_Camera camera;
		
		unsigned int last_step_time = 0;

		
		Vec2D::Vec2D mouse_grid_point;
		
		float temperature = 1.0;
		float temperature_decay = 0.98;
		float permute_amount = 10.0;
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

		generate_New_Solution(World::points.rect, World::Parameters::n_points, 0.5, 0.5, World::maze.n_cols - 1, World::maze.n_rows - 1);
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		Grid_Camera::calibrate(&World::camera);
	
		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			for (int i = 0; i < 1000; i++)
			{
				World::fitness = update_Solution(World::points.rect, World::tmp_points.rect, World::Parameters::n_points, &World::maze, World::permute_amount, World::temperature);
			}
			World::temperature *= World::temperature_decay;
		}

		Vec2D::Vec2D screen_point = { Input::mouse_x ,Input::mouse_y };
		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, &screen_point, &World::camera);
	}

	void draw(unsigned int current_time)
	{
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		SDL_RenderClear(Engine::renderer);

		RGBA::RGBA tile_tint = { 255,255,255,255 };
		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, &tile_tint, Engine::renderer);

		RGB::RGB color = { 255,0,0 };
		Shape::Rect::draw(&World::points, &color, &World::camera, Engine::renderer);
	
		char str[256];
		sprintf(str, "temperature %f\nfitness %f", World::temperature, World::fitness);
		Font::set_Screen_Size(&World::text, 32, 32);
		Font::set_Screen_Pos(&World::text, 10, 10);
		Font::draw(&World::text, str, strlen(str), &World::camera, Engine::renderer);
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

	
}