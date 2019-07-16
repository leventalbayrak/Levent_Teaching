#include <iostream>
#pragma warning(disable:4996)
using namespace std;

class Animal
{
private:
	int hidden;//noone can access except Animal
protected://can be accessed by children if they inherit public or protected
	int health;
public:
	Animal()
	{
		health = 100;
	}
	~Animal()
	{

	}

	void talk()
	{
		cout << "teach me manners, my health is " << health << endl;
	}

};

class Cat : public Animal
{

};

class Bat : public Animal
{
public:
	Bat()
	{
		cout << "i overwrote the constructor (BAT)" << endl;
	}
};

class Rat : public Animal
{
	int mana;
public:
	Rat()
	{
		mana = 10;
		cout << "i overwrote the constructor (RAT)" << endl;
	}
	void talk()
	{
		cout << "IM A RAT, i have " << mana << " mana" << endl;
		cout << "I HAVE " << health << " health" <<endl;
	}
};

int main()
{

	Animal generic_animal;
	generic_animal.talk();

	Animal *p = new Animal();
	p->talk();

	Cat rhengar;
	rhengar.talk();

	Bat batman;
	batman.talk();

	Rat mickey_mouse;
	mickey_mouse.talk();

	getchar();
	return 0;
}