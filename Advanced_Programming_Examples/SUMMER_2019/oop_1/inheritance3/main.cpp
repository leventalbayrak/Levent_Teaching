#include <iostream>
#pragma warning(disable:4996)
using namespace std;

class Animal
{
public:
	Animal()
	{

	}
	~Animal()
	{

	}

	void talk()
	{
		cout << "i am an animal teach me manners" << endl;
	}

	virtual void walk()
	{
		cout << "i float" << endl;
	}

	//this will turn Animal into an abstract class
	//you cannot make an instance of Animal
	//you must derive from it and define die() function in the derived class
	virtual void die() = 0;

};

class Cat : public Animal
{
public:
	void die()
	{
		
	}
};


int main()
{
	//if Animal is abstract
	//Cat will only work if die() is defined inside Cat
	Cat rhengar;
	rhengar.talk();
	rhengar.walk();

	getchar();
	return 0;
}