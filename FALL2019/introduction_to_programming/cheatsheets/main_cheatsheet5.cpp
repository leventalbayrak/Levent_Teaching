#include <stdio.h>
#include <stdlib.h>

struct Box
{
	float x, y, sx, sy, w, h;
};

struct Boxes
{
	float x[100];
	float y[100];
	float sx[100];
	float sy[100];
	float w[100];
	float h[100];
};

int main()
{
	/*
	ARRAY OF STRUCTS
	*/
	Box data[100];//stack
	static Box data2[100];//heap
	data[5].x = 5;

	/*
	STRUCT OF ARRAYS
	*/
	Boxes data3;
	
	getchar();
	return 0;
}






