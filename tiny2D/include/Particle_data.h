#pragma once
#include "Shape_data.h"
#include "Sprite_data.h"
#include "Body_data.h"
namespace Particle
{

	struct Particle_Factory
	{
		Sprite::Factory sprites;
		Body::Factory bodies;
		Shape::Rect::Factory world_coords;

		int *life;
		int n_particles;
		int array_size;
	};

	struct Emitter_Factory
	{
		Body::Factory bodies;
		Shape::Rect::Factory world_coords;
		Particle_Factory particles;
	};
}