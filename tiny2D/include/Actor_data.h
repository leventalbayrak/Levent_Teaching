#pragma once
#include "Shape_data.h"
#include "Sprite_data.h"
#include "Body_data.h"
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
		int n_actors;
		int array_size;
	};

	

}