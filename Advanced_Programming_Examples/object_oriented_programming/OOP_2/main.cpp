#include <stdio.h>
#include <thread>
#include <Windows.h>
using namespace std;

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
		printf("Vehicle constructed %x\n", this);
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

	float get_Max_Speed() 
	{ 
		return max_speed; 
	}
	
	void set_Max_Speed(float s) 
	{
		max_speed = s; 
	}

};

int add()
{
	printf("i am adddinggg!!!!\n");
	Sleep(1000);
	return  0;
}

int sub()
{
	printf("WOAAAAAAAHHHHH SUBTRACTTTTTT WWHAHAATR?????\n");
	return  0;
}

int main()
{
	Vehicle car;
	car.set_Max_Speed(120);

	Vehicle *p = new Vehicle();
	p->set_Max_Speed(130);



	getchar();
	return 0;
}