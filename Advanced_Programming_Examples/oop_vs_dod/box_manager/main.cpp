
#include <stdio.h>
#include <stdlib.h>

class Rect
{
	float x, y, w, h;
public:
	Rect()
	{
		x = 0;
		y = 0;
		w = 1;
		h = 1;
	}
	~Rect()
	{

	}
	void print()
	{
		printf("%f %f %f %f\n", x, y, w, h);
	}
};

class Rect_Manager
{
	Rect **rectangles;
	int array_size;
public:
	Rect_Manager(int _array_size = 100)
	{
		array_size = _array_size;
		rectangles = new Rect*[array_size];
		for (int i = 0; i < array_size; i++) rectangles[i] = NULL;
	}

	~Rect_Manager()
	{
		for (int i = 0; i < array_size; i++)
		{
			if (rectangles[i] != NULL)
			{
				delete rectangles[i];
			}
		}

		delete[] rectangles;
	}

	Rect* create()
	{
		for (int i = 0; i < array_size; i++)
		{
			if (rectangles[i] == NULL)
			{
				rectangles[i] = new Rect();
				return rectangles[i];
			}
		}

		return NULL;
	}

	void destroy(Rect *p)
	{
		for (int i = 0; i < array_size; i++)
		{
			if (rectangles[i] == p)
			{
				delete rectangles[i];
				rectangles[i] = NULL;
				return;
			}
		}
	}

	void print()
	{
		for (int i = 0; i < array_size; i++)
		{
			if (rectangles[i] != NULL)
			{
				rectangles[i]->print();
			}
		}
	}

	Rect *get_First_Active()
	{
		for (int i = 0; i < array_size; i++)
		{
			if (rectangles[i] != NULL)
			{
				return rectangles[i];
			}
		}
		return NULL;
	}

};

int main()
{
	Rect_Manager factory;

	for (int frame = 0; frame < 5; frame++)
	{
		int number_of_rectangles_to_spawn = rand() % 4;
		
		for (int i = 0; i < number_of_rectangles_to_spawn; i++)
		{
			Rect *r = factory.create();
		}

		int number_of_rectangles_to_destroy = rand() % 2;
		for (int i = 0; i < number_of_rectangles_to_destroy; i++)
		{
			Rect *r = factory.get_First_Active();
			factory.destroy(r);
		}

		factory.print();

		printf("tried to spawn %d rectangles\n", number_of_rectangles_to_spawn);
		printf("tried to destroy %d rectangles\n", number_of_rectangles_to_destroy);
	}

	getchar();
	return 0;
}