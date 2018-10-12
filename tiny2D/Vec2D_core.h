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
		float m = a->x*a->x + a->y * a->y;
		if (m == 0.0) return 0.0;
		return sqrt(m);
	}

	inline float unit(Vec2D *a)
	{
		float m = mag(a);
		if (m == 0)
		{
			a->x = 0.0;
			a->y = 0.0;
			return 0.0;
		}
		a->x /= m;
		a->y /= m;
		return m;
	}
}