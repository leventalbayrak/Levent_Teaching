#pragma once
#include "Shape_data.h"
#include "Sprite_data.h"
#include "Body_data.h"
#include "RGBA_data.h"
#include "Spawn_Stack_data.h"
namespace Actor
{

	struct Factory
	{
		Sprite::Factory *sprites;
		int n_sprites;
		int sprite_array_size;

		Body::Factory bodies;
		Shape::Rect::Factory world_coords;

		int *current_sprite;
		unsigned int *creation_time;
		RGBA::RGBA *color;
		int *sprite_flip;
		int array_size;

		Spawn_Stack::Spawn_Stack spawn_stack;
	};
}