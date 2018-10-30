#pragma once
#include "Shape_data.h"
#include "Sprite_data.h"
#include "Body_data.h"
#include "Spawn_Stack_data.h"
namespace Particle
{

	struct Emitter
	{
		Vec2D::Vec2D emitter_world_coords;

		Sprite::Factory sprites;
		Body::Factory bodies;
		Shape::Rect::Factory world_coords;

		unsigned int *creation_time;
		int *life_span;//in milliseconds
		int array_size;

		Spawn_Stack::Spawn_Stack spawn_stack;

	};

}