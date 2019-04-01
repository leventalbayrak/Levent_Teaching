#include <stdio.h>

class Vehicle
{
	int number_of_wheels;
	float engine_size;
	float fuel_capacity;
	float current_fuel;
	float max_speed;

public:

	//constructor
	Vehicle()
	{
		number_of_wheels = 4;
		engine_size = 2.0;
		fuel_capacity = 15.0;
		current_fuel = 0.0;
		max_speed = 100.0;
		printf("Vehicle constructed %x\n",this);
	}

	Vehicle(int model)
	{
		if (model == 1)
		{
			number_of_wheels = 4;
			engine_size = 2.0;
			fuel_capacity = 15.0;
			current_fuel = 0.0;
			max_speed = 100.0;
			printf("model %d\n", model);
		}
		else
		{
			number_of_wheels = 8;
			engine_size = 3.0;
			fuel_capacity = 12.0;
			current_fuel = 0.0;
			max_speed = 200.0;
			printf("model %d\n", model);
		}
		printf("Vehicle constructed %x\n", this);
	}

	//destructor
	~Vehicle()
	{
		printf("Vehicle is desconstructed %x\n", this);
	}

};


int main()
{

	Vehicle car;

	if (true)
	{
		Vehicle another_car;
	}

	Vehicle *p;
	Vehicle *s = new Vehicle();
	delete s;

	if (true)
	{
		Vehicle z[5];
	}

	Vehicle bike_A(1);
	Vehicle bike_B(2);

	Vehicle *b = new Vehicle(1);
	delete b;

	Vehicle *f = new Vehicle[2];
	delete []f;

	printf("Vehicle **v = new Vehicle* [2];\n");
	Vehicle **v = new Vehicle* [2];
	printf("v[0] = new Vehicle(2);\n");
	v[0] = new Vehicle(2);
	printf("v[1] = new Vehicle();\n");
	v[1] = new Vehicle();

	for (int i = 0; i < 2; i++)
	{
		printf("delete v[i];\n");
		delete v[i];
	}
	printf("delete[] v;\n");
	delete[] v;


	getchar();
	return 0;
}