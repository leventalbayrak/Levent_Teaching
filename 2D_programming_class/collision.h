#pragma once

#include "rect.h"

int checkCollision(Rect obj1, Rect obj2)
{
	enum { NO_COLLISION = 0, TOP_OF_B, RIGHT_OF_B, BOTTOM_OF_B, LEFT_OF_B };

	float w = 0.5 * (obj1.w + obj2.w);
	float h = 0.5 * (obj1.h + obj2.h);
	float dx = obj1.x - obj2.x + 0.5*(obj1.w - obj2.w);
	float dy = obj1.y - obj2.y + 0.5*(obj1.h - obj2.h);

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
			return (wy + hx > 0) ? TOP_OF_B : RIGHT_OF_B;
		}
	}
	return NO_COLLISION;
}