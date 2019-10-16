//standard input output library - needed for printf
#include <stdio.h>


//ENTRY POINT
//PROGRAM STARTS HERE
int main()
{//scope begin
	/*
	VARIABLES
	*/
	int x = 5;//4 bytes
	float y1 = 0.2f;//4 bytes
	float y2 = 0.2;
	char letter = 'A';//1 byte

	/*
	BRANCHING
	*/
	if (x < 4)//inside: either 0 or NON-ZERO
	{
		printf("x < 4\n");
	}

	if (x == 5)
	{
		printf("x is 5\n");
	}
	else
	{
		printf("x is not 5\n");
	}

	if (x < 3)
	{
		printf("x is less than 3\n");
	}
	else if (x < 4)
	{
		printf("x is less than 4\n");
	}
	else if (x < 6)
	{
		printf("x is less than 6\n");
	}
	else
	{
		printf("x is greater than and equal to 6\n");
	}

	/*
	SWITCH 
	*/
	int option = 8;
	switch (option)
	{
		case 1:
		//code for when option is 1
		break;
		case 2:
		//code for when option is 2
		break;
		case 5:
		//code for when option is 5
		break;
		case 6:
		//code for 6
		case 7:
		//code for 7
		break;
		case 'a':
		case 'A':
		//do something when option is a or A
		break;
	default:
		//no case is matched 
		break;
	}

	/*
	PREFIX AND POSTFIX INCREMENT
	*/
	int a = 5;
	int b = ++a;//increment a first, then assign to b
	int c = b++;//assign b to c first, then increment b
	printf("a:%d b:%d c:%d\n", a, b, c);

	/*
	LOOPS
	*/

	//FOR LOOP
	for (int i = 0; i < 5; ++i)//i++ is also correct
	{
		printf("*");
	}
	printf("\n");

	//WHILE LOOP
	int counter = 0;
	while (counter < 5)
	{
		printf("*");
		counter++;
	}
	printf("\n");

	//DO WHILE LOOP
	int k = 0;
	do
	{
		printf("*");
		k++;
	} while (k < 5);

	getchar();
	return 0;
}//scope end


