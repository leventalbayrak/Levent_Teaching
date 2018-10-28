#pragma once
#include "Particle_data.h"
#include "Body_core.h"
#include "Shape_core.h"
#include "Sprite_core.h"
#include "Grid_Camera_core.h"

namespace Particle
{
	void init(Emitter *e, const char *filename, int array_size, SDL_Renderer *renderer)
	{
		e->array_size = array_size;

		Body::init(&e->bodies, array_size);
		
		Sprite::Instance::init(&e->sprites.instances, array_size);
		Sprite::File::add(&e->sprites, filename, renderer);
		Shape::Rect::init(&e->world_coords, array_size);
		e->creation_time = (unsigned int*)malloc(sizeof(unsigned int)*array_size);
		memset(e->creation_time, 0, sizeof(unsigned int)*array_size);
		e->life_span = (int*)malloc(sizeof(int)*array_size);
		memset(e->life_span, 0, sizeof(int)*array_size);
		e->state = (int*)malloc(sizeof(int)*array_size);
		memset(e->state, 0, sizeof(int)*array_size);
		e->emitter_world_coords = {};
		for (int i = 0; i < array_size; i++)
		{
			Body::make_Instance(&e->bodies);
			Sprite::make_Instance(&e->sprites);
			Shape::Rect::make_Instance(&e->world_coords);
			e->state[i] = 0;
			e->creation_time[i] = 0;
		}

	}

	int spawn(Emitter *e, int how_many, const Vec2D::Vec2D *pos, const Vec2D::Vec2D *force_min, const Vec2D::Vec2D *force_max,int min_life, int max_life, unsigned int current_time)
	{
		int spawned_count = 0;
		e->emitter_world_coords = *pos;
		for (int i = 0; i < e->array_size; i++)
		{
			if (e->state[i] == 0)
			{
				e->state[i] = 1;

				e->life_span[i] = min_life + (max_life - min_life) * (double)rand() / RAND_MAX;

				e->creation_time[i] = current_time;
				e->world_coords.rect[i].x = e->emitter_world_coords.x;
				e->world_coords.rect[i].y = e->emitter_world_coords.y;
				e->world_coords.rect[i].w = 1.0;
				e->world_coords.rect[i].h = 1.0;
				e->bodies.pos[i].x = e->emitter_world_coords.x;
				e->bodies.pos[i].y = e->emitter_world_coords.y;
				e->bodies.force[i].x = force_min->x + (force_max->x - force_min->x)*(double)rand() / RAND_MAX;
				e->bodies.force[i].y = force_min->y + (force_max->y - force_min->y)*(double)rand() / RAND_MAX;
				e->bodies.vel[i] = {};
				e->bodies.mass[i] = 1.0;

				e->sprites.instances.current_frame[i] = 0;
				e->sprites.instances.frame_duration[i] = 100;

				if (++spawned_count >= how_many) break;
			}
		}

		return spawned_count;
	}

	void update_Vel_and_Life(Emitter *e, unsigned int current_time, float dt)
	{
		for (int i = 0; i < e->array_size; i++)
		{
			if (current_time - e->creation_time[i] >= e->life_span[i])
			{
				e->state[i] = 0;
			}

			if (e->state[i] == 1)
			{				
				Body::update_Vel(i, &e->bodies, dt);		
				//clear forces
				e->bodies.force[i] = {};
			}
		}
	}

	void apply_Force(Emitter *e, const Vec2D::Vec2D *force)
	{
		for (int i = 0; i < e->array_size; i++)
		{
			if (e->state[i] == 1)
			{
				Body::add_Force(i, &e->bodies, force);
			}
		}
	}

	void update_Pos(Emitter *e, unsigned int current_time, float dt)
	{
		for (int i = 0; i < e->array_size; i++)
		{
			if (e->state[i] == 1)
			{
				Body::update_Pos(i, &e->bodies, dt);
				//copy physics body position to world coord
				e->world_coords.rect[i].x = e->bodies.pos[i].x;
				e->world_coords.rect[i].y = e->bodies.pos[i].y;
			}
		}
	}

	void draw(Emitter *e, const Grid_Camera::Grid_Camera *cam, unsigned int current_time, SDL_Renderer *renderer)
	{
		for (int i = 0; i < e->array_size; i++)
		{
			if (e->state[i] == 1)
			{
				Sprite::update(i, &e->sprites, current_time);

				Shape::Rect::Data screen_rect;
				Grid_Camera::grid_to_Screen(&screen_rect, &e->world_coords.rect[i], cam);

				float alpha = 1.0 - (float)(current_time - e->creation_time[i]) / e->life_span[i];

				Sprite::draw(i, &e->sprites,
					screen_rect.x,
					screen_rect.y,
					screen_rect.w,
					screen_rect.h,
					renderer,
					0,
					255, 255, 255, alpha*255);
			}
		}
	}
}