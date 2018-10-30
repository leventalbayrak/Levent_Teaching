#pragma once
#include "Light_data.h"
#include "Grid_Camera_core.h"
#include "Spawn_Stack_core.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>

namespace Light
{
	namespace System
	{
		void init(int screen_w, int screen_h, SDL_Renderer *renderer);
	}

	void init(Factory *f, int array_size);

	int make_Instance(Factory *f);

	void draw(Factory *l, Grid_Camera::Grid_Camera *cam, SDL_Renderer *renderer);

	void set_Color(int which_light, RGB::RGB *rgb, Factory *l);

	void set_Pos(int which_light, Vec2D::Vec2D *pos, Factory *l);

	void set_Z_Height(int which_light, float z_height, Factory *l);

	void set_Intensity(int which_light, float intensity, Factory *l);
}

namespace Light
{
	namespace System
	{
		void init(int screen_w, int screen_h, SDL_Renderer *renderer)
		{
			internal::lightmap = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
			assert(internal::lightmap);
			internal::w = screen_w;
			internal::h = screen_h;
			SDL_SetTextureBlendMode(internal::lightmap, SDL_BLENDMODE_MOD);
		}
	}

	void init(Factory *f, int array_size)
	{
		f->array_size = array_size;
		
		f->color = (RGB::RGB*)malloc(sizeof(RGB::RGB)*f->array_size);
		f->pos = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*f->array_size);
		f->z_height = (float*)malloc(sizeof(float)*f->array_size);
		f->intensity = (float*)malloc(sizeof(float)*f->array_size);

		Spawn_Stack::init(&f->spawn_stack, array_size);
	}

	int make_Instance(Factory *f)
	{
		return Spawn_Stack::make(&f->spawn_stack);
	}

	void draw(Factory *l, Grid_Camera::Grid_Camera *cam, SDL_Renderer *renderer)
	{
		int pitch = 0;
		unsigned char* pixels = NULL;

		SDL_LockTexture(internal::lightmap, NULL, (void**)&pixels, &pitch);

		int buffer_size = pitch * internal::h;

		for (int index = 0; index < buffer_size; index += 4)
		{
			int x = (index % pitch) / 4;
			int y = index / pitch;

			double r = 0;
			double g = 0;
			double b = 0;

			//convert texture coord to world coord
			Vec2D::Vec2D tex_pixel_pos = { x,y };
			Vec2D::Vec2D grid_pixel_pos;
			Grid_Camera::screen_to_Grid(&grid_pixel_pos, &tex_pixel_pos, cam);

			Vec3D::Vec3D surface_normal = { 0,0,1 };
			float pixel_height = 0.0;
			Vec3D::Vec3D frag_pos = { grid_pixel_pos.x, grid_pixel_pos.y, pixel_height };

			for (int k = 0; k < l->spawn_stack.array_size; k++)
			{
				if (l->spawn_stack.spawned[k] == 0) continue;

				Vec3D::Vec3D light_pos = { l->pos[k].x, l->pos[k].y, l->z_height[k] };

				Vec3D::Vec3D light_dir = light_pos;
				Vec3D::sub(&light_dir, &frag_pos);
				Vec3D::norm(&light_dir);

				float diffuse = max(Vec3D::dot(&surface_normal, &light_dir), (float)0.0);

				r += l->color[k].r * (diffuse + ambient);
				g += l->color[k].g * (diffuse + ambient);
				b += l->color[k].b * (diffuse + ambient);
			}

			pixels[index + 0] = 0;//dont touch alpha
			pixels[index + 1] = min((int)r, 255);
			pixels[index + 2] = min((int)g, 255);
			pixels[index + 3] = min((int)b, 255);

		}

		SDL_UnlockTexture(internal::lightmap);
		SDL_RenderCopy(renderer, internal::lightmap, NULL, NULL);
	}

	void set_Color(int which_light, RGB::RGB *rgb, Factory *l)
	{
		l->color[which_light] = *rgb;
	}

	void set_Pos(int which_light, Vec2D::Vec2D *pos, Factory *l)
	{
		l->pos[which_light] = *pos;
	}

	void set_Z_Height(int which_light, float z_height, Factory *l)
	{
		l->z_height[which_light] = z_height;
	}

	void set_Intensity(int which_light, float intensity, Factory *l)
	{
		l->intensity[which_light] = intensity;
	}
}