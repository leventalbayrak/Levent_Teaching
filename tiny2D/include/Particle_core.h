#pragma once
#include "Particle_data.h"
#include "Body_core.h"
#include "Shape_core.h"
#include "Sprite_core.h"
#include "Grid_Camera_core.h"
#include "Spawn_Stack_core.h"

namespace Particle
{

	void init(Emitter *e, const char *filename, int array_size, SDL_Renderer *renderer);

	int spawn(Emitter *e, int how_many, float scale, const Vec2D::Vec2D *pos, const Vec2D::Vec2D *initial_vel, const Vec2D::Vec2D *force_min, const Vec2D::Vec2D *force_max, int min_life, int max_life, unsigned int current_time);

	void update_Vel_and_Life(Emitter *e, unsigned int current_time, float dt);

	void apply_Force(Emitter *e, const Vec2D::Vec2D *force);

	void update_Pos(Emitter *e, unsigned int current_time, float dt);

	int draw(Emitter *e, const Grid_Camera::Grid_Camera *cam, unsigned int current_time, SDL_Renderer *renderer);
}

namespace Particle
{

	void init(Emitter *e, const char *filename, int array_size, SDL_Renderer *renderer)
	{
		e->array_size = array_size;

		Spawn_Stack::init(&e->spawn_stack, array_size);

		Body::init(&e->bodies, array_size);
		
		Sprite::Instance::init(&e->sprites.instances, array_size);
		Sprite::add(&e->sprites, filename, renderer);

		Shape::Rect::init(&e->world_coords, array_size);
		
		e->creation_time = (unsigned int*)malloc(sizeof(unsigned int)*array_size);
		memset(e->creation_time, 0, sizeof(unsigned int)*array_size);
		
		e->life_span = (int*)malloc(sizeof(int)*array_size);
		memset(e->life_span, 0, sizeof(int)*array_size);
		
		e->emitter_world_coords = {};
		
		for (int i = 0; i < array_size; i++)
		{
			Body::make_Instance(&e->bodies);
			Sprite::make_Instance(&e->sprites);
			Shape::Rect::make_Instance(&e->world_coords);
			e->creation_time[i] = 0;
		}

	}

	int spawn(Emitter *e, int how_many, float scale, const Vec2D::Vec2D *pos, const Vec2D::Vec2D *initial_vel, const Vec2D::Vec2D *force_min, const Vec2D::Vec2D *force_max,int min_life, int max_life, unsigned int current_time)
	{
		int spawned_count = 0;

		e->emitter_world_coords = *pos;
		for (int i = 0; i < how_many; i++)
		{
			int k = Spawn_Stack::make(&e->spawn_stack);
			
			if (k < 0) break;

			spawned_count++;

			e->life_span[k] = min_life + (max_life - min_life) * (double)rand() / RAND_MAX;

			e->creation_time[k] = current_time;
			e->world_coords.rect[k].x = e->emitter_world_coords.x;
			e->world_coords.rect[k].y = e->emitter_world_coords.y;
			e->world_coords.rect[k].w = scale;
			//TODO: WATCH THIS
			e->world_coords.rect[k].h = scale * (float)e->sprites.texture_info.frame_h / e->sprites.texture_info.frame_w;
			e->bodies.pos[k].x = e->emitter_world_coords.x;
			e->bodies.pos[k].y = e->emitter_world_coords.y;
			e->bodies.force[k].x = force_min->x + (force_max->x - force_min->x)*(double)rand() / RAND_MAX;
			e->bodies.force[k].y = force_min->y + (force_max->y - force_min->y)*(double)rand() / RAND_MAX;
			e->bodies.vel[k] = *initial_vel;
			e->bodies.mass[k] = 1.0;

			e->sprites.instances.current_frame[k] = 0;
			e->sprites.instances.frame_duration[k] = 100;
		}

		return spawned_count;
	}

	void update_Vel_and_Life(Emitter *e, unsigned int current_time, float dt)
	{
		for (int k = 0; k < e->spawn_stack.array_size; k++)
		{
			if (e->spawn_stack.spawned[k] == 0) continue;
			
			if (current_time - e->creation_time[k] >= e->life_span[k])
			{
				Spawn_Stack::destroy(k, &e->spawn_stack);
			}
			else
			{
				Body::update_Vel(k, &e->bodies, dt);
				//clear forces
				e->bodies.force[k] = {};
			}

		}
	}

	void apply_Force(Emitter *e, const Vec2D::Vec2D *force)
	{
		for (int k = 0; k < e->spawn_stack.array_size; k++)
		{
			if (e->spawn_stack.spawned[k] == 1)
			{
				Body::add_Force(k, &e->bodies, force);
			}
		}
	}

	void update_Pos(Emitter *e, unsigned int current_time, float dt)
	{
		for (int k = 0; k < e->spawn_stack.array_size; k++)
		{
			if (e->spawn_stack.spawned[k] == 1)
			{
				Body::update_Pos(k, &e->bodies, dt);
				//copy physics body position to world coord
				e->world_coords.rect[k].x = e->bodies.pos[k].x;
				e->world_coords.rect[k].y = e->bodies.pos[k].y;
			}
		}
	}

	int draw(Emitter *e, const Grid_Camera::Grid_Camera *cam, unsigned int current_time, SDL_Renderer *renderer)
	{
		
		int n_drawn = 0;
		for (int k = 0; k < e->spawn_stack.array_size; k++)
		{
			if (e->spawn_stack.spawned[k] == 1)
			{
				Sprite::update(k, &e->sprites, current_time);

				Shape::Rect::Data screen_rect;
				Grid_Camera::grid_to_Screen(&screen_rect, &e->world_coords.rect[k], cam);

				float alpha = 1.0 - (float)(current_time - e->creation_time[k]) / e->life_span[k];

				Sprite::draw(k, &e->sprites,
					screen_rect.x,
					screen_rect.y,
					screen_rect.w,
					screen_rect.h,
					renderer,
					0,
					255, 255, 255, alpha * 255);

				n_drawn++;
			}
		}

		return n_drawn;
	}
}