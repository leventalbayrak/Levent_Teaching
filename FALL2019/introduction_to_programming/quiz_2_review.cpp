/*
QUIZ 2 Review
Write a program that converts degrees in Fahrenheit to Celcius.
- The user must be able enter Fahrenheit values using the console (scanf)
- The conversion must be made using a function.
Use: float F_to_C(float f);
Formula: Celcius = (Fahrenheit - 32.0) * 5.0 / 9.0;
*/

#include <stdio.h>
#pragma warning(disable:4996)

float F_to_C(float f)
{
	float c = (f - 32.0f)*5.0f / 9.0f;
	return c;
}

int main()
{
	//get user input
	float fahrenheit = 0;
	printf("please enter degrees in Fahrenheit:");
	scanf("%f", &fahrenheit);

	//call F_to_C function and print out the result
	float celcius = F_to_C(fahrenheit);
	printf("%f Fahrenheit is %f degrees Celcius\n", fahrenheit, celcius);
	
	getchar();
	getchar();
	return 0;
}