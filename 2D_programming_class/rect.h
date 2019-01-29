#pragma once

#include "draw.h"

struct Rect
{
	ColorSet color;
	float x, y;
	int w, h;
};

void fill_Rect(Rect rect, int screen_w, int screen_H, ColorSet *grid)
{
	if (rect.x < 0) rect.x = 0;
	if (rect.x + rect.w > screen_w) rect.w = screen_w - rect.x - 1;
	if (rect.y < 0) rect.y = 0;
	if (rect.y + rect.h > screen_H) rect.h = screen_H - rect.y - 1;

	for (int i = rect.y; i < rect.y + rect.h; i++)
	{
		for (int j = rect.x; j < rect.x + rect.w; j++)
		{
			draw(&grid[j + (i * screen_w)], rect.color);
		}
	}
}