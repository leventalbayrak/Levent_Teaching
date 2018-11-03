#include <iostream>
#include <Windows.h>
using namespace std;

class Box
{
	float x, y, w, h, sx, sy;
	int r, g, b;
public:
	Box()
	{
	}
	~Box()
	{
	}
	void update()
	{
		x += sx;
		y += sy;
	}
};

struct Box_Pos
{
	float x, y;
};
struct Box_Size
{
	float w, h;
};
struct Box_Speed
{
	float sx, sy;
};

void update(Box_Pos *p, Box_Speed *s)
{
	p->x += s->sx;
	p->y += s->sy;
}

int main(int argc, char **argv)
{

	int n_boxes = 2000000;
	Box_Pos *box_pos = new Box_Pos[n_boxes];
	Box_Size *box_size = new Box_Size[n_boxes];
	Box_Speed *box_speed = new Box_Speed[n_boxes];
	
	Box *boxes = new Box[n_boxes];

	LARGE_INTEGER t1, t2, t3;

	QueryPerformanceCounter(&t1);

	for (int i = 0; i < n_boxes; i++)
	{
		boxes[i].update();
	}

	QueryPerformanceCounter(&t2);

	for (int i = 0; i < n_boxes; i++)
	{
		update(&box_pos[i], &box_speed[i]);
	}
	QueryPerformanceCounter(&t3);

	cout << t2.QuadPart - t1.QuadPart << endl;
	cout << t3.QuadPart - t2.QuadPart << endl;

	getchar();
	return 0;
}
	