#include <iostream>
#pragma warning(disable:4996)
using namespace std;

class Friend
{
	//private member variables
	int health;
	int mana;
	char *name;
public:
	//constructor (startup code)
	Friend()
	{
		name = new char[30];
		strcpy(name, "noone");
		cout << "constructing enemy" << endl;
	}
	Friend(const char* n)
	{
		name = new char[30];
		strcpy(name, n);
		cout << "constructing " << n << " enemy" << endl;
	}

	//destructor (ending code)
	~Friend()
	{
		cout << "destructing enemy " << name << endl;
		delete[] name;
	}

	//getter
	int get_Health()
	{
		return health;
	}
	int get_Mana()
	{
		return mana;
	}
	char* get_Name()
	{
		return name;
	}

	//setter
	void set_Health(int h)
	{
		health = h;
	}

	void set_Mana(int m)
	{
		mana = m;
	}
};

int main()
{
	Friend mark;
	Friend chinh("yo");

	if(true)
	{
		Friend terence("autochess");
	}

	cout << "mark's name is " << mark.get_Name() << endl;
	
	if (true)
	{
		cout << "stack array" << endl;
		Friend party[5];
	}

	if (true)
	{
		cout << "new array" << endl;
		Friend *party = new Friend[5];

		delete[] party;
	}

	if (true)
	{
		cout << "malloc array" << endl;
		Friend *party = (Friend*)malloc(sizeof(Friend) * 5);
		free(party);
	}


	getchar();
	return 0;
}