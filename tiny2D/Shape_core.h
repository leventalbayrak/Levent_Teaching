#pragma once
#include "Shape_data.h"

namespace Shape
{
	int collision(const Rect *pa, const Rect *sa, const Rect *pb, const Rect *sb)
	{
		if (pa->x + sa->w < pb->x) return 0;
		if (pb->x + sb->w < pa->x) return 0;
		if (pa->y + sa->h < pb->y) return 0;
		if (pb->y + sb->h < pa->y) return 0;
		return 1;
	}

	//minkowski sum
	//if collision direction is important, use this
	int collision_with_Dir(const Rect *pa, const Rect *sa, const Rect *pb, const Rect *sb)
	{
		enum { NO_COLLISION = 0, TOP_OF_B, RIGHT_OF_B, BOTTOM_OF_B, LEFT_OF_B };

		float w = 0.5 * (sa->w + sb->w);
		float h = 0.5 * (sa->h + sb->h);
		float dx = pa->x - pb->x + 0.5*(sa->w - sb->w);
		float dy = pa->y - pb->y + 0.5*(sa->h - sb->h);

		if (dx*dx <= w * w && dy*dy <= h * h)
		{
			float wy = w * dy;
			float hx = h * dx;

			if (wy > hx)
			{
				return (wy + hx > 0) ? BOTTOM_OF_B : LEFT_OF_B;
			}
			else
			{
				return (wy + hx > 0) ? RIGHT_OF_B : TOP_OF_B;
			}
		}
		return NO_COLLISION;
	}
}