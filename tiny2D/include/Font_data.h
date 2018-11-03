#pragma once
#include "Tileset_data.h"
#include "Shape_data.h"
#include "RGBA_data.h"
namespace Font
{
	struct Font
	{
		Shape::Rect::Data screen_coord;
		Tileset::Tileset tileset;
		RGBA::RGBA color;
	};
}