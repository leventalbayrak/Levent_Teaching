#pragma once
#include "Shape_data.h"
#include "Vec2D_data.h"
namespace Body
{
	struct Body
	{
		int n_bodies;
		int array_size;

		Vec2D::Vec2D *force;
		Vec2D::Vec2D *pos;
		Vec2D::Vec2D *dir;
		float *mass;
		float *speed;
	};
}