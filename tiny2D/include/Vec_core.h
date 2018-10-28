#pragma once
#include "Vec_data.h"
#include <math.h>
namespace Vec2D
{
	inline void add(Vec2D *a, const Vec2D *b)
	{
		a->x += b->x;
		a->y += b->y;
	}

	inline void sub(Vec2D *a, const Vec2D *b)
	{
		a->x -= b->x;
		a->y -= b->y;
	}

	inline float dot(const Vec2D *a, const Vec2D *b)
	{
		return a->x*b->x + a->y*b->y;
	}

	inline float dist_Squared(const Vec2D *a, const Vec2D *b)
	{
		return (a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y);
	}

	inline float mag(const Vec2D *a)
	{
		float m = a->x*a->x + a->y * a->y;
		if (m == 0.0) return 0.0;
		return sqrt(m);
	}

	inline float norm(Vec2D *a)
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

namespace Vec3D
{

	inline void add(Vec3D *a, const Vec3D *b)
	{
		a->x += b->x;
		a->y += b->y;
		a->z += b->z;
	}

	inline void sub(Vec3D *a, const Vec3D *b)
	{
		a->x -= b->x;
		a->y -= b->y;
		a->z -= b->z;
	}

	inline float dot(const Vec3D *a, const Vec3D *b)
	{
		return a->x*b->x + a->y*b->y + a->z*b->z;
	}

	inline float dist_Squared(const Vec3D *a, const Vec3D *b)
	{
		return (a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y) + (a->z - b->z)*(a->z - b->z);
	}

	inline float mag(const Vec3D *a)
	{
		float m = a->x*a->x + a->y * a->y + a->z*a->z;
		if (m == 0.0) return 0.0;
		return sqrt(m);
	}

	inline float norm(Vec3D *a)
	{
		float m = mag(a);
		if (m == 0)
		{
			a->x = 0.0;
			a->y = 0.0;
			a->z = 0.0;
			return 0.0;
		}
		a->x /= m;
		a->y /= m;
		a->z /= m;
		return m;
	}
}