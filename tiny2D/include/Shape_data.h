#pragma once
#include "Spawn_Stack_data.h"
namespace Shape
{
	namespace Rect
	{
		struct Data
		{
			float x, y, w, h;
		};

		struct Factory
		{
			Data *rect;
			int array_size;
			Spawn_Stack::Spawn_Stack spawn_stack;
		};
	}
}

