#pragma once
#include "Shape_data.h"
#include "Vec_data.h"
#include "Spawn_Stack_data.h"
namespace Body
{
	struct Factory
	{
		int array_size;
		Spawn_Stack::Spawn_Stack spawn_stack;

		Vec2D::Vec2D *force;
		Vec2D::Vec2D *pos;
		Vec2D::Vec2D *last_pos;
		Vec2D::Vec2D *vel;
		float *mass;
	};
}