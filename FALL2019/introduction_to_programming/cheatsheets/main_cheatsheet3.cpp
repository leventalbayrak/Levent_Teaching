#include <stdio.h>
#include <stdlib.h>

//RECIPE FOR THE STRUCT
struct Box
{
	float x, y;
	float w, h;
};

//makes a copy of the box being passed to the function
void print_BAD(Box copy_of_box)
{
	printf("%f %f %f %f\n", copy_of_box.x, copy_of_box.y, copy_of_box.w, copy_of_box.h);
}

void print_BETTER(Box* p)
{
	printf("%f %f %f %f\n", p->x, p->y, p->w, p->h);
}

void modify_CANT(Box a)
{
	a.w *= 2;
	a.h *= 2;
}

void modify_CAN(Box* a)
{
	a->w *= 2;
	a->h *= 2;
}

void square_CANT(int x)
{
	x *= x;//x is a copy of the integer
}

void square_CAN(int* x)
{
	*x *= *x;//x is the address, *x grabs the data
}

int main()
{
	Box a;
	a.x = 5;
	a.y = 6;
	a.w = 4;
	a.h = 4;

	Box b = { 5, 6, 4, 4 };

	Box c = { 0 };

	print_BAD(a);

	print_BETTER(&a);

	printf("before b.w: %f\n", b.w);
	modify_CANT(b);
	printf("after b.w: %f\n", b.w);

	printf("before b.w: %f\n", b.w);
	modify_CAN(&b);
	printf("after b.w: %f\n", b.w);

	int money = 200;
	printf("money: %d\n", money);
	square_CANT(money);
	printf("money: %d\n", money);

	printf("money: %d\n", money);
	square_CAN(&money);
	printf("money: %d\n", money);
	
	getchar();
	return 0;
}






