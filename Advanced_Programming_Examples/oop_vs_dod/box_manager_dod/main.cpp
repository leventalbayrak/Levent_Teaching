
#include <stdio.h>
#include <stdlib.h>

struct Rect
{
	int active;
	float x, y, w, h;
};

int create(Rect *rectangles, int array_size)
{
	for (int i = 0; i < array_size; i++)
	{
		if (rectangles[i].active == 0)
		{
			rectangles[i].active = 1;
			return i;
		}
	}

	return -1;
}

void destroy(int which, Rect *rectangles, int array_size)
{
	rectangles[which].active = 0;
}

void print(Rect *rectangles, int array_size)
{
	for (int i = 0; i < array_size; i++)
	{
		if (rectangles[i].active == 1)
		{
			printf("%f %f %f %f\n", rectangles[i].x, rectangles[i].y, rectangles[i].w, rectangles[i].h);
		}
	}
}

int main()
{
	int array_size = 100;
	Rect *rectangles = (Rect*)malloc(sizeof(Rect)*array_size);

	int r = create(rectangles, array_size);
	destroy(r, rectangles, array_size);

	return 0;
}