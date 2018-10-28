#pragma once
#include "Shape_data.h"
#include "Vec_data.h"
namespace Body
{
	struct Factory
	{
		int n_bodies;
		int array_size;

		Vec2D::Vec2D *force;
		Vec2D::Vec2D *pos;
		Vec2D::Vec2D *last_pos;
		Vec2D::Vec2D *vel;
		float *mass;
	};
}