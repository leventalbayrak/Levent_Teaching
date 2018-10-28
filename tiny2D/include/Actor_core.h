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

		int find_New(Factory *f)
		{
			for (int i = 0; i < f->array_size; i++)
			{
				if (f->state[i] == -1) return i;
			}
			return -1;
		}
	}

	void init(Factory *f, int array_size)
	{
		Body::init(&f->bodies, array_size);
		
		Shape::Rect::init(&f->world_coords, array_size);

		f->array_size = array_size;
		f->n_sprites = 0;
		f->sprite_array_size = 16;
		f->sprites = (Sprite::Factory*)malloc(sizeof(Sprite::Factory)*f->sprite_array_size);
		f->current_sprite = (int*)malloc(sizeof(int)*array_size);
		f->creation_time = (unsigned int*)malloc(sizeof(unsigned int)*array_size);
		f->state = (int*)malloc(sizeof(int)*array_size);
		f->sprite_flip = (int*)malloc(sizeof(int)*array_size);
		f->color = (RGBA::RGBA*)malloc(sizeof(RGBA::RGBA)*array_size);

		for (int i = 0; i < f->sprite_array_size; i++) Sprite::Instance::init(&f->sprites[i].instances, array_size);

		for (int i = 0; i < array_size; i++)
		{
			f->current_sprite[i] = 0;
			f->creation_time[i] = 0;
			f->sprite_flip[i] = 0;
			f->state[i] = -1;
			f->color[i] = { 255,255,255,255 };
		}
		
	}

	void add_Sprite(Factory *f, const char *filename, SDL_Renderer *renderer)
	{
		if (f->n_sprites >= f->sprite_array_size)
		{
			internal::resize_Sprites(f);
		}
		Sprite::File::add(&f->sprites[f->n_sprites], filename, renderer);
		f->n_sprites++;
	}

	void add_Force(int which_actor, Factory *f, Vec2D::Vec2D *force)
	{
		Body::add_Force(which_actor, &f->bodies, force);
	}

	void update_Vel(int which_actor, Factory *f, float dt)
	{
		Body::update_Vel(which_actor, &f->bodies, dt);
		f->bodies.force[which_actor] = {};
	}

	void update_Pos(int which_actor, Factory *f, float dt)
	{
		Body::update_Pos(which_actor, &f->bodies, dt);
		//copy physics body position to world coord
		f->world_coords.rect[which_actor].x = f->bodies.pos[which_actor].x;
		f->world_coords.rect[which_actor].y = f->bodies.pos[which_actor].y;
	}

	void undo_Pos_Update(int which_actor, Factory *f)
	{
		f->bodies.pos[which_actor] = f->bodies.last_pos[which_actor];
		//copy physics body position to world coord
		f->world_coords.rect[which_actor].x = f->bodies.pos[which_actor].x;
		f->world_coords.rect[which_actor].y = f->bodies.pos[which_actor].y;
	}

	void apply_Friction(int which_actor, const Vec2D::Vec2D *friction, Factory *f)
	{
		Body::apply_Friction(which_actor, friction, &f->bodies);
	}

	void set_Sprite(int which_actor, int which_sprite, Factory *f)
	{
		f->current_sprite[which_actor] = which_sprite;
	}
	
	void set_Pos(int which_actor,float x, float y, Factory *f)
	{
		f->bodies.last_pos[which_actor] = f->bodies.pos[which_actor];

		f->world_coords.rect[which_actor].x = x;
		f->world_coords.rect[which_actor].y = y;
		
		f->bodies.pos[which_actor].x = x;
		f->bodies.pos[which_actor].y = y;
	}

	void set_Vel(int which_actor, const Vec2D::Vec2D *vel, Factory *f)
	{
		f->bodies.vel[which_actor] = *vel;
	}

	Vec2D::Vec2D *get_Vel(int which_actor, Factory *f)
	{
		return &f->bodies.vel[which_actor];
	}
	Vec2D::Vec2D *get_Pos(int which_actor, Factory *f)
	{
		return &f->bodies.pos[which_actor];
	}
	Vec2D::Vec2D *get_Last_Pos(int which_actor, Factory *f)
	{
		return &f->bodies.last_pos[which_actor];
	}

	void set_Size(int which_actor, float w, float h, Factory *f)
	{
		f->world_coords.rect[which_actor].w = w;
		f->world_coords.rect[which_actor].h = h;
	}

	void set_Scale(int which_actor, Factory *f, float scale)
	{
		f->world_coords.rect[which_actor].w *= scale;
		f->world_coords.rect[which_actor].h *= scale;
	}

	int spawn(Factory *f, float scale, unsigned int current_time)
	{
		int k = internal::find_New(f);
		if (k == -1) return -1;

		f->state[k] = 0;

		f->creation_time[k] = current_time;

		f->current_sprite[k] = 0;

		int k0 = 0;
		for (int j = 0; j < f->n_sprites; j++)
		{
			k0 = Sprite::make_Instance(&f->sprites[j]);
			Sprite::modify(k0, &f->sprites[j], 120);
		}

		int k1 = Body::make_Instance(&f->bodies);
		f->bodies.pos[k1] = { 0,0 };
		f->bodies.mass[k1] = 1.0;
		f->bodies.force[k1] = {};
		f->bodies.vel[k1] = {};

		int k2 = Shape::Rect::make_Instance(&f->world_coords);
		

		f->world_coords.rect[k2].x = 0;
		f->world_coords.rect[k2].y = 0;
		f->world_coords.rect[k2].w = scale;
		//TODO: WATCH THIS
		f->world_coords.rect[k2].h = scale*(float)f->sprites[0].texture_info.frame_h / f->sprites[0].texture_info.frame_w;

		return k;
	}

	int draw(const Factory *f, Grid_Camera::Grid_Camera *cam, unsigned int current_time, SDL_Renderer *renderer)
	{
		int n_drawn = 0;
		for (int i = 0; i < f->array_size; i++)
		{
			if (f->state[i] != -1)
			{
				Sprite::update(i, &f->sprites[f->current_sprite[i]], current_time);

				Shape::Rect::Data screen_rect;
				Grid_Camera::grid_to_Screen(&screen_rect, &f->world_coords.rect[i], cam);

				Sprite::draw(i, &f->sprites[f->current_sprite[i]],
					screen_rect.x,
					screen_rect.y,
					screen_rect.w,
					screen_rect.h,
					renderer,
					f->sprite_flip[i],
					f->color[i].r, f->color[i].g, f->color[i].b, f->color[i].a);

				n_drawn++;
			}
		}

		return n_drawn;
	}

}