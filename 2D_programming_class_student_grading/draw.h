#include "core.h"

void color_Copy(Color_Set *destination, Color_Set source)
{
	destination->r = source.r;
	destination->g = source.g;
	destination->b = source.b;
}

void fill_Rect(Rect rect, Color_Set *canvas, int canvas_w, int canvas_h)
{
	for (int i = rect.position.y; i < rect.position.y + rect.size.y; i++)
	{
		for (int j = rect.position.x; j < rect.position.x + rect.size.x; j++)
		{
			color_Copy(&canvas[(i * canvas_w) + j], rect.color);
		}
	}
}