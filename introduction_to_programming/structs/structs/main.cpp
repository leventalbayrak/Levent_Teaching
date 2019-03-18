#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

struct Car
{
	float zero_sixty;
	int horsepower;
	char symbol;
	char *make;
};

int main()
{

	int x = 6;
	
	Car y;
	y.zero_sixty = 5.3;
	y.horsepower = 250;
	y.make = "Subaru";
	y.symbol = 'S';

	Car z = { 3.9, 550, 'D', "Dodge" };

	printf("sizeof(Car) is %d\n", sizeof(Car));

	Car *dealership = (Car *)malloc(sizeof(Car) * 1000);

	int n_cars = 0;
	FILE *f = fopen("cars.txt", "r");
	for (;;)
	{
		Car tmp_car = { 0 };
		tmp_car.make = (char *)malloc(sizeof(char) * 100);
		int r = fscanf(f, "%f\t%d\t%c\t%s", &tmp_car.zero_sixty, &tmp_car.horsepower, &tmp_car.symbol, tmp_car.make);
		if (r != 4) break;

		dealership[n_cars] = tmp_car;
		n_cars++;
	}

	for (int i = 0; i < n_cars; i++)
	{
		printf("CAR[%d] symbol=%c make=%s zerosixty=%f hp=%d\n",i, dealership[i].symbol, dealership[i].make, dealership[i].zero_sixty, dealership[i].horsepower);
	}

	getchar();
	return 0;
}