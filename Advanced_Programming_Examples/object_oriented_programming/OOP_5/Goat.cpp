#include "Goat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Goat::Goat()
{
	printf("Goat::Goat()\n");
}

void Goat::hunt()
{
	eat(0);
	printf("joke's on you goats can't hunt\n");
}
