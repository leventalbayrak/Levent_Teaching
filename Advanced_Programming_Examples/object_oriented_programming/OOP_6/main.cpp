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

	virtual void walk()
	{
		printf("Animal::walk()\n");
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
		printf("Goat::hunt()\n");
		eat(0);
		printf("joke's on you goats can't hunt\n");
	}

	void walk()
	{
		printf("Goat::walk()\n");
	}

};

class Lion : public Animal
{

public:
	Lion()
	{
		printf("Lion::Lion()\n");
		printf("a real animal\n");
	}

	void hunt()
	{
		printf("Lion::hunt()\n");
		eat(1000);	
	}

	void walk()
	{
		printf("Lion::walk()\n");
	}

};

int main()
{
	srand(time(0));

	Goat shrek;
	Lion mufasa;

	Animal *ptr1 = &shrek;
	Animal *ptr2 = &mufasa;

	printf("---hunt---\n");
	ptr1->hunt();
	ptr2->hunt();

	printf("---walk---\n");
	ptr1->walk();
	ptr2->walk();

	Animal *zoo[2] = { &shrek,&mufasa };
	zoo[0]->walk();
	zoo[1]->walk();

	getchar();
	return 0;
}