#include <stdio.h>
#include <stdlib.h>

//FUNCTION WITH NO RETURN VALUE, NO ARGUMENTS
void function_A()
{
	printf("function_A\n");
}

//FUNCTION WITH NO RETURN VALUE, HAS ARGUMENTS
void function_B(int x, int y, char c)
{
	printf("%d %d %c\n", x, y, c);
}

//FUNCTION WITH A RETURN VALUE, NO ARGUMENTS
int function_C()
{
	return 1 + rand() % 6;
}

//FUNCTION WITH A RETURN VALUE AND ARGUMENTS
float function_D(float a, float b)
{
	float avg = 0.5*(a + b);
	return avg;
}

//THIS IS A FUNCTION
int main()
{
	function_A();

	int x = 9;
	function_B(x, 12, 'A');

	int y = function_C();

	float avg1 = function_D(2.0, 3.0);
	
	float avg2 = function_D(function_C(), function_C());

	getchar();
	return 0;
}


