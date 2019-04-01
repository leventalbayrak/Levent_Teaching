#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Animal.h"

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
