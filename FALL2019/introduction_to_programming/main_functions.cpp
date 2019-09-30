#include <stdio.h>

void solid_line()
{
	printf("* * * * *\n");
}

void star()
{
	printf("*");
}

void newline()
{
	printf("\n");
}

void empty_line()
{
	printf("       ");
}

void star_line()
{
	star();
	empty_line();
	star();
	newline();
}

void box()
{
	solid_line();
	star_line();
	star_line();
	star_line();
	star_line();
	solid_line();
}

int main()
{

	box();

	getchar();

	return 0;
}