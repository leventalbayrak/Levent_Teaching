#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

class Box
{
	float x, y, w, h;
};

//STORES BOXES
namespace BM //for box manager
{
	struct BM
	{
		Box *boxes;
		int n_boxes;
		int array_size;
	};

	void init(BM *b)
	{
		b->array_size = 1000;
		b->n_boxes = 0;
		b->boxes = new Box[b->array_size];
	}

	Box* create(BM *b)
	{
		if (b->n_boxes >= b->array_size)
		{
			b->array_size *= 1.2;
			Box *tmp = new Box[b->array_size];
			memcpy(b->boxes, tmp, sizeof(Box)*b->n_boxes);
			delete[] b->boxes;
			b->boxes = tmp;
		}
		b->n_boxes++;
		return &b->boxes[b->n_boxes - 1];
	}

}

//STORES ADDRESSES OF BOXES PASSED BY THE USER
namespace BM_OOP
{
	struct BM_OOP
	{
		Box **boxes;
		int n_boxes;
		int array_size;
	};

	void init(BM_OOP *b)
	{
		b->array_size = 1000;
		b->n_boxes = 0;
		b->boxes = new Box*[b->array_size];
	}

	void add(BM_OOP *b, Box *box)
	{
		if (b->n_boxes >= b->array_size)
		{
			b->array_size *= 1.2;
			Box **tmp = new Box*[b->array_size];
			memcpy(b->boxes, tmp, sizeof(Box*)*b->n_boxes);
			delete[] b->boxes;
			b->boxes = tmp;
		}
		b->boxes[b->n_boxes++] = box;
	}
}

int main()
{
	//example 1
	BM::BM boxes1;
	BM::init(&boxes1);

	Box *tmp1 = BM::create(&boxes1);
	//modify tmp1
	

	//example 2
	BM_OOP::BM_OOP boxes2;
	BM_OOP::init(&boxes2);

	//allocate first, then pass the pointer
	Box *tmp2 = new Box();
	BM_OOP::add(&boxes2, tmp2);

	return 0;
}