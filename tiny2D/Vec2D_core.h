#pragma once
#include "Vec2D_data.h"
#include <math.h>
namespace Vec2D
{
	inline void add(Vec2D *a, Vec2D *b)
	{
		a->x += b->x;
		a->y += b->y;
	}

	inline float mag(Vec2D *a)
	{
		return sqrt(a->x*a->x + a->y * a->y);
	}

	inline float unit(Vec2D *a)
	{
		float m = mag(a);
		a->x /= m;
		a->y /= m;
		return m;
	}
}