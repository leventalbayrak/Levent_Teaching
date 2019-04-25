
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

int main()
{

	Rect **rectangles = new Rect* [100];
	for (int i = 0; i < 100; i++) rectangles[i] = NULL;

	for (int frame = 0; frame < 5; frame++)
	{
		int number_of_rectangles_to_spawn = rand() % 4;
		//spawn rectangles
		for (int i = 0; i < number_of_rectangles_to_spawn; i++)
		{
			//1- find available position
			//2- allocate and record the address at that position
			for (int j = 0; j < 100; j++)
			{
				if (rectangles[j] == NULL)
				{
					rectangles[j] = new Rect();
					break;
				}
			}
		}

		int number_of_rectangles_to_destroy = rand() % 2;
		//destroy rectangles
		//1- find active rectangle position
		//2- delete the rectangle at the address and make rectangle pointer NULL
		int counter = 0;
		for (int i = 0; i < 100; i++)
		{
			if (counter == number_of_rectangles_to_destroy) break;

			if (rectangles[i] != NULL)
			{
				delete rectangles[i];
				rectangles[i] = NULL;
				counter++;
			}
		}

		

		int number_of_active_rectangles = 0;
		//print rectangles that are alive
		for (int i = 0; i < 100; i++)
		{
			if (rectangles[i] != NULL)
			{
				number_of_active_rectangles++;
				rectangles[i]->print();
			}
		}

		printf("tried to spawn %d rectangles\n", number_of_rectangles_to_spawn);
		printf("tried to destroy %d rectangles\n", number_of_rectangles_to_destroy);
		printf("number of active rectangles = %d\n", number_of_active_rectangles);
	}

	getchar();
	return 0;
}