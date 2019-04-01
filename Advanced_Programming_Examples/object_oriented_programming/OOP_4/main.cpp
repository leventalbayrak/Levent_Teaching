#include <stdio.h>
#include <stdlib.h>
#include <time.h>
class Animal
{
	int energy;

	void digest(int e)
	{
		printf("Animal::digest()\n");
		energy += 0.9*e;
		printf("energy: %d\n", energy);
	}

protected:
	void eat(int e)
	{
		printf("Animal::eat()\n");
		energy -= 10;
		digest(e);

	}

public:
	Animal()
	{
		printf("Animal::Animal()\n");
		energy = 100;

	}

	~Animal()
	{
		printf("Animal::~Animal()\n");
	}

	void hunt()
	{
		printf("Animal::hunt()\n");

		int energy_contained_in_prey = 10 + rand() % 100;
		int energy_required_to_catch = 0.75*energy_contained_in_prey;
		int caught = rand() % 2;

		energy -= energy_required_to_catch;
		if (caught == 1)
		{
			eat(energy_contained_in_prey);
			printf("hunt success!\n");
		}
	}

};

class Goat : public Animal
{

public:
	Goat()
	{
		printf("Goat::Goat()\n");
	}

	void hunt()
	{
		eat(0);
		printf("joke's on you goats can't hunt\n");
	}

};


int main()
{
	srand(time(0));

	Goat goat;

	for (;;)
	{
		goat.hunt();
		getchar();
	}

	return 0;
}