#pragma once
#include "Shape_data.h"
#include "Spawn_Stack_core.h"

namespace Shape
{
	namespace Rect
	{
		void init(Factory *f, int array_size);

		int make_Instance(Factory *f);

		int collision(const Data *pa, const Data *pb);

		//minkowski sum
		//if collision direction is important, use this
		int collision_with_Dir(const Data *a, const Data *b);
	}

}

namespace Shape
{
	namespace Rect
	{
		void init(Factory *f, int array_size)
		{
			f->array_size = array_size;
			f->rect = (Data*)malloc(sizeof(Data)*f->array_size);

			Spawn_Stack::init(&f->spawn_stack, array_size);
		}

		int make_Instance(Factory *f)
		{
			return Spawn_Stack::make(&f->spawn_stack);
		}

		int collision(const Data *pa, const Data *pb)
		{
			if (pa->x + pa->w < pb->x) return 0;
			if (pa->x > pb->x + pb->w) return 0;
			if (pa->y + pa->h < pb->y) return 0;
			if (pa->y > pb->y + pb->h) return 0;
			return 1;
		}

		//minkowski sum
		//if collision direction is important, use this
		int collision_with_Dir(const Data *a, const Data *b)
		{
			//                           1        2           3             4
			enum { NO_COLLISION = 0, TOP_OF_B, RIGHT_OF_B, BOTTOM_OF_B, LEFT_OF_B };

			float w = 0.5 * (a->w + b->w);
			float h = 0.5 * (a->h + b->h);
			float dx = a->x - b->x + 0.5*(a->w - b->w);
			float dy = a->y - b->y + 0.5*(a->h - b->h);

			if (dx*dx <= w * w && dy*dy <= h * h)
			{
				float wy = w * dy;
				float hx = h * dx;

				if (wy > hx)
				{
					return (wy + hx > 0) ? BOTTOM_OF_B : LEFT_OF_B;
				}
				else
				{
					return (wy + hx > 0) ? RIGHT_OF_B : TOP_OF_B;
				}
			}
			return NO_COLLISION;
		}


		void draw(Factory *f, RGB::RGB *color, const Grid_Camera::Grid_Camera *cam, SDL_Renderer *renderer)
		{
			for (int i = 0; i < f->array_size; i++)
			{
				if (f->spawn_stack.spawned[i] == 0) continue;

				Shape::Rect::Data screen_rect;
				Grid_Camera::grid_to_Screen(&screen_rect, &f->rect[i], cam);

				SDL_Rect dest;
				dest.x = screen_rect.x;
				dest.y = screen_rect.y;
				dest.w = screen_rect.w;
				dest.h = screen_rect.h;

				SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
				SDL_RenderFillRect(renderer, &dest);
			}
		}

		void draw_Connecting_Lines(Factory *f, RGB::RGB *color, const Grid_Camera::Grid_Camera *cam, SDL_Renderer *renderer)
		{
			int last = 0;
			for (int i = 0; i < f->array_size; i++)
			{
				if (f->spawn_stack.spawned[i] == 1)
				{
					last = i;
					break;
				}
			}

			Shape::Rect::Data screen_rect_last;
			Grid_Camera::grid_to_Screen(&screen_rect_last, &f->rect[last], cam);
			
			for (int i = last + 1; i < f->array_size; i++)
			{
				if (f->spawn_stack.spawned[i] == 0) continue;

				Shape::Rect::Data screen_rect_current;
				Grid_Camera::grid_to_Screen(&screen_rect_current, &f->rect[i], cam);

				SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
				SDL_RenderDrawLine(renderer, screen_rect_last.x + screen_rect_last.w*0.5, screen_rect_last.y + screen_rect_last.h*0.5, screen_rect_current.x + screen_rect_current.w*0.5, screen_rect_current.y + screen_rect_current.h*0.5);

				screen_rect_last = screen_rect_current;
			}
		}
	}
	
}