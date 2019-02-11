#pragma once

struct Float_Pair
{
	float x, y;
};

struct Color_Set
{
	unsigned char r, g, b, a;
};

struct Rect
{
	Float_Pair position;
	Float_Pair size;
	Color_Set color;
};

struct Entity
{
	Rect body;
	Float_Pair velocity;
	float mass;
};

void fp_Add(Float_Pair *a, Float_Pair b)
{
	a->x += b.x;
	a->y += b.y;
}

void fp_Sub(Float_Pair *a, Float_Pair b)
{
	a->x -= b.x;
	a->y -= b.y;
}

void fp_Scale(Float_Pair *pair, float scale)
{
	pair->x *= scale;
	pair->y *= scale;
}

float fp_Dot(Float_Pair a, Float_Pair b)
{
	return a.x * b.x + a.y + b.y;
}