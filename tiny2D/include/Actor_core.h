#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "Actor_data.h"
#include "Sprite_core.h"
#include "Body_core.h"
#include "Shape_core.h"
#include "RGBA_data.h"

namespace Actor
{
	namespace internal
	{
		void resize_Sprites(Factory *f)
		{
			f->sprite_array_size += f->sprite_array_size >> 1;
			f->sprites = (Sprite::Factory*)realloc(f->sprites, sizeof(Sprite::Factory)*f->sprite_array_size);
			for (int i = f->n_sprites; i < f->sprite_array_size; i++) Sprite::Instance::init(&f->sprites[i].instances, f->sprites[0].instances.array_size);
		}
	}

	void init(Factory *f, int array_size)
	{
		Body::init(&f->bodies, array_size);
		
		Shape::Rect::init(&f->world_coords, array_size);

		f->array_size = array_size;
		f->n_actors = 0;
		f->n_sprites = 0;
		f->sprite_array_size = 16;
		f->sprites = (Sprite::Factory*)malloc(sizeof(Sprite::Factory)*f->sprite_array_size);
		f->current_sprite = (int*)malloc(sizeof(int)*array_size);
		
		for (int i = 0; i < f->sprite_array_size; i++) Sprite::Instance::init(&f->sprites[i].instances, array_size);
	}

	void add(Factory *f, const char *filename, SDL_Renderer *renderer)
	{
		if (f->n_sprites >= f->sprite_array_size)
		{
			internal::resize_Sprites(f);
		}
		Sprite::File::add(&f->sprites[f->n_sprites], filename, renderer);
		f->n_sprites++;
	}

	void update_Vel(int which_instance, Factory *f, float dt)
	{
		Body::update_Vel(which_instance, &f->bodies, dt);
	}

	void update_Pos(int which_instance, Factory *f, float dt)
	{
		Body::update_Pos(which_instance, &f->bodies, dt);
		//copy physics body position to world coord
		f->world_coords.rect[which_instance].x = f->bodies.pos[which_instance].x;
		f->world_coords.rect[which_instance].y = f->bodies.pos[which_instance].y;
	}

	void set_Sprite(int which_instance, int which_sprite, Factory *f)
	{
		f->current_sprite[which_instance] = which_sprite;
	}
	
	void set_Pos(int which_instance,float x, float y, Factory *f)
	{
		f->world_coords.rect[which_instance].x = x;
		f->world_coords.rect[which_instance].y = y;
		f->bodies.pos[which_instance].x = x;
		f->bodies.pos[which_instance].y = y;
	}

	void set_Size(int which_instance, float w, float h, Factory *f)
	{
		f->world_coords.rect[which_instance].w = w;
		f->world_coords.rect[which_instance].h = h;
	}

	int make_Instance(Factory *f)
	{

		if (f->n_actors >= f->array_size)
		{
			f->array_size += f->array_size >> 1;
			f->current_sprite = (int*)realloc(f->current_sprite, sizeof(int)*f->array_size);
		}

		f->current_sprite[f->n_actors] = 0;

		int k0 = 0;
		for (int i = 0; i < f->n_sprites; i++)
		{
			k0 = Sprite::make_Instance(&f->sprites[i]);
			Sprite::modify(k0, &f->sprites[i], 120);
		}

		int k1 = Body::make_Instance(&f->bodies);
		f->bodies.pos[k1] = { 0,0 };
		f->bodies.mass[k1] = 1.0;
		f->bodies.force[k1] = {};
		f->bodies.vel[k1] = {};

		int k2 = Shape::Rect::make_Instance(&f->world_coords);
		f->world_coords.rect[k2] = { 0,0,1.0,1.0 };
		f->world_coords.rect[k2].x = 0;
		f->world_coords.rect[k2].y = 0;
		
		
		assert(k0 == k1);
		assert(k0 == k2);
		assert(f->n_actors == k0);
	
		++f->n_actors;
		return f->n_actors - 1;
	}

	void update_Sprite_Frame(int which_instance, const Factory *f, unsigned int current_time)
	{
		Sprite::update(which_instance, &f->sprites[f->current_sprite[which_instance]], current_time);

	}

	void draw(int which_instance, const Factory *f,int flip, RGBA::RGBA *rgba,Grid_Camera::Grid_Camera *cam, SDL_Renderer *renderer)
	{
		Shape::Rect::Data screen_rect;
		Grid_Camera::grid_to_Screen(&screen_rect, &f->world_coords.rect[which_instance], cam);

		Sprite::draw(which_instance, &f->sprites[f->current_sprite[which_instance]],
			screen_rect.x,
			screen_rect.y,
			screen_rect.w,
			screen_rect.h,
			renderer,
			flip,
			rgba->r, rgba->g, rgba->b, rgba->a);
	}

}