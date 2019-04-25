#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Goat
{
	int ugliness;
public:
	Goat()
	{
		ugliness = 5 + rand() % 10;
	}

	Goat operator+(Goat &a)
	{
		Goat baby;
		baby.ugliness = ugliness + a.ugliness;
		return baby;
	}

	int operator[](int i)
	{
		return rand()%i;
	}

	int show_Real_Face() { return ugliness; }
};

class A
{
protected:
	int a;
public:
	
	virtual void test() = 0;
};

class B : public A
{
public:
	B() { a = 92; }
	void test()
	{
		printf("hahah %d\n",a);
	}
};


int main()
{

	B b;
	b.test();
	getchar();

	Goat sally;
	Goat bob;

	Goat baby = sally + bob;

	printf("mama goat ugliness level: %d\n", sally.show_Real_Face());
	printf("papa goat ugliness level: %d\n", bob.show_Real_Face());
	printf("baby ugliness level: %d\n", baby.show_Real_Face());

	printf("baby array????????: %d\n", baby[5]);
	printf("baby array????????: %d\n", baby[5]);
	printf("baby array????????: %d\n", baby[5]);
	printf("baby array????????: %d\n", baby[5]);

	baby.operator[](5);

	getchar();
	return 0;
}