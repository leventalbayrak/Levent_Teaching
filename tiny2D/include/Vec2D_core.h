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

	void clip(Vec2D *a, float min_x, float max_x, float min_y, float max_y)
	{
		if (a->x < min_x) a->x = min_x;
		if (a->x > max_x) a->x = max_x;
		if (a->y < min_y) a->y = min_y;
		if (a->y > max_y) a->y = max_y;
	}
}