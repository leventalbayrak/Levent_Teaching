#include <stdio.h>


int main()
{
	int x = 5;
	float y = 0.99f;
	char c = 'A';

	printf("x is %d\n", x);
	printf("y is %f\n", y);
	printf("c is %c\n", c);

	int* apple = &x;
	float* orange = &y;
	char* pineapple = &c;

	printf("address of x is %u\n", apple);
	printf("address of y is %u\n", orange);
	printf("address of c is %u\n", pineapple);

	printf("address of apple is %u\n", &apple);
	printf("address of orange is %u\n", &orange);
	printf("address of pineapple is %u\n", &pineapple);

	printf("*apple is %d\n", *apple);
	printf("*orange is %f\n", *orange);
	printf("*pineapple is %c\n", *pineapple);

	getchar();
	return 0;
}