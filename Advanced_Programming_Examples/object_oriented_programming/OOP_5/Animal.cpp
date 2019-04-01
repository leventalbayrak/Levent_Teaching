#include "Animal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Animal::digest(int e)
{
	printf("Animal::digest()\n");
	energy += 0.9*e;
	printf("energy: %d\n", energy);
}


void Animal::eat(int e)
{
	printf("Animal::eat()\n");
	energy -= 10;
	digest(e);

}

Animal::Animal()
{
	printf("Animal::Animal()\n");
	energy = 100;

}

Animal::~Animal()
{
	printf("Animal::~Animal()\n");
}

void Animal::hunt()
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
