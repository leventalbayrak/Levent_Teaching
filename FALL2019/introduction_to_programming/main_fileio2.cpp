#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

struct Box
{
	float x, y, w, h, r, g, b;
};

void load_Box(Box* p, const char* filename)
{
	FILE* f = fopen(filename, "r");
	fscanf(f, "%f", &p->x);
	fscanf(f, "%f", &p->y);
	fscanf(f, "%f", &p->w);
	fscanf(f, "%f", &p->h);
	fscanf(f, "%f", &p->r);
	fscanf(f, "%f", &p->g);
	fscanf(f, "%f", &p->b);
	fclose(f);
}

int main()
{
	
	Box a = { 0 };

	load_Box(&a, "box.txt");

	printf("%f %f %f %f %f %f %f\n", a.x, a.y, a.w, a.h, a.r, a.g, a.b);
	
	getchar();
	return 0;
}






