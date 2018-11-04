#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"



namespace My_Game
{

	void generate_New_Solution(Shape::Rect::Data *shapes, int n_shapes, float w, float h, float max_x, float max_y)
	{
		shapes[0] = { (float)1.0,(float)(max_y / 2.0),w,h };
		shapes[n_shapes - 1] = { (float)(max_x - 2.0),(float)(max_y / 2.0),w,h };
		//shapes[n_shapes - 1] = { (float)(max_x - 2.0),(float)(max_y - 2.0),w,h };
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

		for (int i = 0; i < n_shapes; i++)
		{
			for (int j = i + 1; j < n_shapes; j++)
			{
				int r = Shape::Rect::collision(&shapes[i], &shapes[j]);
				if (r == 1)
				{
					fitness -= 100;
				}
			}
		}

		for (int i = 1; i < n_shapes - 1; i++)
		{
			int k = Grid::get_First_Overlapped_Tile(-1, &shapes[i], collision);
			if (k != -1)
			{
				fitness -= -100;
			}

			double dist_from_neighbor_mid = 0.0;
			double x = 0.5*(shapes[i - 1].x + shapes[i + 1].x);
			double y = 0.5*(shapes[i - 1].y + shapes[i + 1].y);
			dist_from_neighbor_mid += (shapes[i].x - x)*(shapes[i].x - x);
			dist_from_neighbor_mid += (shapes[i].y - y)*(shapes[i].y - y);

			//fitness -= 4.0*sqrt(dist_from_neighbor_mid);		
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
			int k = 1 + rand() % (n_shapes - 2);
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

		_permute_Solution(tmp, src, n_shapes, permute_amount, collision->n_cols - 1, collision->n_rows - 1);

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

	float solve(Shape::Rect::Data *src, Shape::Rect::Data *tmp, int n_shapes, int n_steps_per_temperature, Grid::Grid *collision, float permute_amount, float temperature, float temperature_decay)
	{
		int n_steps = 0;
		float fitness = 0.0;
		float epsilon = 0.0001;
		while (temperature >= epsilon)
		{
			for (int i = 0; i < n_steps_per_temperature; i++)
			{
				fitness = update_Solution(src, tmp, n_shapes, collision, permute_amount, temperature);

				n_steps++;
			}
			temperature *= temperature_decay;

		}
		//printf("n_steps = %d\n", n_steps);
		return fitness;
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
			int n_points = 100;
			float box_size = 1.0;
			float permute_amount = 24.0;//<-- VERY IMPORTANT RANK 3
			float temperature_decay = 0.95;//<-- VERY IMPORTANT RANK 1
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

		generate_New_Solution(World::points.rect, World::Parameters::n_points, World::Parameters::box_size, World::Parameters::box_size, World::maze.n_cols - 1, World::maze.n_rows - 1);
#define DEBUG_EXPERIMENT 0
#if DEBUG_EXPERIMENT 1
		FILE *f = fopen("result.txt", "w+");
		float temp_min = 0.95;
		float temp_max = 0.95;
		float temp_step = 0.01;
		float permute_min = 24.0;
		float permute_max = 24.0;
		float permute_step = 8.0;

		int n_permute_same_temp_min = 5;
		int n_permute_same_temp_max = 990;
		int n_permute_same_temp_step = 2;

		for (float p = permute_min; p <= permute_max; p += permute_step)
		{

			for (float t = temp_min; t <= temp_max; t += temp_step)
			{
				for (int ps = n_permute_same_temp_min; ps <= n_permute_same_temp_max; ps *= n_permute_same_temp_step)
				{
					printf("%d - %f - %f\n", ps, p, t);
					generate_New_Solution(World::points.rect, World::Parameters::n_points, World::Parameters::box_size, World::Parameters::box_size, World::maze.n_cols - 1, World::maze.n_rows - 1);
					float fitness = solve(World::points.rect, World::tmp_points.rect, World::Parameters::n_points, ps, &World::maze, p, 1.0, t);
					fprintf(f, "%d\t%f\t%f\t%f\n", ps, fitness, p, t);
				}
			}

		}

		fclose(f);
		exit(0);
#endif

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		Grid_Camera::calibrate(&World::camera);

		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			for (int i = 0; i < World::Parameters::n_permute_same_temp; i++)
			{
				World::fitness = update_Solution(World::points.rect, World::tmp_points.rect, World::Parameters::n_points, &World::maze, World::Parameters::permute_amount, World::temperature);
			}
			World::temperature *= World::Parameters::temperature_decay;
		}

		Vec2D::Vec2D screen_point = { Input::mouse_x ,Input::mouse_y };
		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, &screen_point, &World::camera);


		if (Input::mouse_left == 1)
		{
			World::temperature = 1.0;

			Vec2D::Vec2D tmp = { World::points.rect[World::Parameters::n_points - 1].x,World::points.rect[World::Parameters::n_points - 1].y };
			generate_New_Solution(World::points.rect, World::Parameters::n_points, World::Parameters::box_size, World::Parameters::box_size, World::maze.n_cols - 1, World::maze.n_rows - 1);

			World::points.rect[World::Parameters::n_points - 1].x = tmp.x;
			World::points.rect[World::Parameters::n_points - 1].y = tmp.y;
			World::points.rect[0].x = World::mouse_grid_point.x;
			World::points.rect[0].y = World::mouse_grid_point.y;
		}

		if (Input::mouse_right == 1)
		{
			World::temperature = 1.0;

			Vec2D::Vec2D tmp = { World::points.rect[0].x,World::points.rect[0].y };
			generate_New_Solution(World::points.rect, World::Parameters::n_points, World::Parameters::box_size, World::Parameters::box_size, World::maze.n_cols - 1, World::maze.n_rows - 1);

			World::points.rect[0].x = tmp.x;
			World::points.rect[0].y = tmp.y;
			World::points.rect[World::Parameters::n_points - 1].x = World::mouse_grid_point.x;
			World::points.rect[World::Parameters::n_points - 1].y = World::mouse_grid_point.y;
		}

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