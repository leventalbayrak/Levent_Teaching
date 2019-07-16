#include <iostream>
#pragma warning(disable:4996)
using namespace std;

class A
{
	int x;
protected:
	int y;

public:
	A()
	{
		y = 3;
	}
	virtual int get()
	{
		return y;
	}
};

class B : public A
{
	int x;
	int y;
public:
	B()
	{
		y = 7;
	}
	int get()
	{
		//you can access A's y using
		//A::y
		return y;
	}
};

int main()
{
	A a;
	B b;

	A *p[2];
	p[0] = &a;
	p[1] = &b;

	cout << p[0]->get() << " " << p[1]->get() << endl;

	getchar();
	return 0;
}