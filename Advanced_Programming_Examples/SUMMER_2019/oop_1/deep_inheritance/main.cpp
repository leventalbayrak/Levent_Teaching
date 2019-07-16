#include <iostream>
#pragma warning(disable:4996)
using namespace std;

class A
{
public:
	virtual void hello()
	{
		cout << "A" << endl;
	}
};

class B : public A
{
public:
	virtual void hello()
	{
		cout << "B" << endl;
	}
};

class C : public B
{
public:
	void hello()
	{
		cout << "C" << endl;
	}
};

int main()
{
	A a;
	B b;
	C c;
	A *data[3];
	data[0] = &a;
	data[1] = &b;
	data[2] = &c;

	data[0]->hello();
	data[1]->hello();
	data[2]->hello();

	getchar();
	return 0;
}