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
	//virtual void die() = 0;

};

class Cat : public Animal
{
public:
	void talk()
	{
		cout << "i am a cat, do not teach me manners" << endl;
	}
	
	void walk()
	{
		cout << "i sneak because im untrustworthy" << endl;
	}
};

class Bat : public Animal
{
public:
	void talk()
	{
		cout << "i am a Bat what?" << endl;
	}

	void walk()
	{
		cout << "i fly coz i can" << endl;
	}
};


int main()
{
	//if Animal is abstract, this will not work
	Animal generic_animal;
	generic_animal.talk();
	generic_animal.walk();

	//if Animal is abstract, this will not work
	Animal *p = new Animal();
	p->talk();
	p->walk();

	//if Animal is abstract
	//Cat will only work if die() is defined inside Cat
	Cat rhengar;
	rhengar.talk();
	rhengar.walk();

	//if base class pointer accessesa virtual function, 
	//the child class function will be used
	Animal *important = &rhengar;
	important->talk();
	important->walk();

	Cat marks_cat;
	Bat chinhs_bat;
	Animal *zoo[2];

	zoo[0] = &marks_cat;
	zoo[1] = &chinhs_bat;
	cout << "zoo" << endl;
	for (int i = 0; i < 2; i++)
	{
		zoo[i]->walk();
	}

	getchar();
	return 0;
}