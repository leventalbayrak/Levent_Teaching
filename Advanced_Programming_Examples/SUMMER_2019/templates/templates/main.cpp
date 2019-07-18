#include <iostream>
using namespace std;

struct Ball
{
	int x, y;
};

void swap_using_pointer(int* a, int* b, Ball *ball)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;

	Ball tmp_ball = { 0 };
	int x = ball->x + tmp_ball.x;

}

void swap_using_reference(int& a, int& b, Ball& ball)
{
	int tmp = a;
	a = b;
	b = tmp;

	Ball tmp_ball = { 0 };
	int x = ball.x + tmp_ball.x;
}

int main()
{
	int a = 2;
	int b = 7;

	Ball c;

	cout << a << " " << b << endl;
	swap_using_pointer(&a, &b, &c);
	cout << a << " " << b << endl;

	cout << a << " " << b << endl;
	swap_using_reference(a, b, c);
	cout << a << " " << b << endl;

	getchar();
	return 0;
}