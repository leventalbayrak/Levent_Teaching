#pragma once
#include "draw.h"

struct Rect
{
	ColorSet color;
	double x, y, w, h;
};

void fillRect(Rect rect, int screenW, int screenH, ColorSet *grid)
{
	if (rect.x < 0) rect.x = 0;
	if (rect.x + rect.w > screenW) rect.w = screenW - rect.x - 1;
	if (rect.y < 0) rect.y = 0;
	if (rect.y + rect.h > screenH) rect.h = screenH - rect.y - 1;

	for (int i = rect.y; i < rect.y + rect.h; i++)
	{
		for (int j = rect.x; j < rect.x + rect.w; j++)
		{
			draw(&grid[j + (i * screenW)], rect.color);
		}
	}
}