#pragma once

struct ColorSet
{
	unsigned char r, g, b, a;
};

inline void draw(ColorSet *pixel, ColorSet color)
{
	//pixel->r = color.r;
	//pixel->g = color.g;
	//pixel->b = color.b;
	//pixel->a = color.a;

	*pixel = color;
}

void drawLine(int x0, int x1, int y0, int y1, int screenW, int screenH, ColorSet *grid, ColorSet color)
{
	int lessX = 0;
	int greatX = 0;
	int lessY = 0;
	int greatY = 0;
	if (x0 == x1)
	{
		if (y0 < y1)
		{
			lessY = y0;
			greatY = y1;
		}
		else
		{
			lessY = y1;
			greatY = y0;
		}
		for (int i = lessY; i <= greatY; i++) draw(&grid[x0 + (i * screenW)], color);
	}
	if (x0 < x1)
	{
		lessX = x0;
		greatX = x1;
		lessY = y0;
		greatY = y1;
	}
	else
	{
		lessX = x1;
		greatX = x0;
		lessY = y1;
		greatY = y0;
	}
	if (y0 == y1)
	{
		for (int i = lessX; i <= greatX; i++) draw(&grid[i + (y0 * screenW)], color);
	}
	int dx = 0;
	int dy = 0;
	dx = greatX - lessX;
	dy = greatY - lessY;
	int y = 0;
	for (int x = lessX; x < greatX; x++)
	{
		if (x < 0) x = 0;
		if (y >= 0)
		{
			if (y >= screenH) break;
			if (x >= screenW) break;
			y = lessY + dy * (x - lessX) / dx;
			draw(&grid[x + (y * screenW)], color);
		}
	}
}


void setColorWhite(ColorSet* color)
{
	color->r = 255;
	color->g = 255;
	color->b = 255;
}

void setColorBlack(ColorSet* color)
{
	color->r = 0;
	color->g = 0;
	color->b = 0;
}

void randomColorLight(ColorSet* color)
{
	color->r = rand() % 255;
	color->g = rand() % 255;
	color->b = rand() % 255;
	int maxColor = rand() % 3;
	if (maxColor == 0) color->r = 255;
	else if (maxColor == 1) color->g = 255;
	else color->b = 255;
}

void randomColorDark(ColorSet* color)
{
	color->r = rand() % 255;
	color->g = rand() % 255;
	color->b = rand() % 255;
	int maxColor = rand() % 3;
	if (maxColor == 0) color->r = 0;
	else if (maxColor == 1) color->g = 0;
	else color->b = 0;
}