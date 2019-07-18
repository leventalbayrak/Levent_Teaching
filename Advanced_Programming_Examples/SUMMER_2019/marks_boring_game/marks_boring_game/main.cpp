#include <iostream>
using namespace std;

class Shop
{
protected:
	char *name;
	float gold_sec;
	float price;
	int level;
public:
	virtual float get_Income() = 0;
	virtual void surprise(float &income) = 0;
	virtual void raise_Level() = 0;

	void print()
	{
		cout << name << " " << get_Income() << endl;
	}
};

class Lemonade_Stand : public Shop
{
public:
	Lemonade_Stand()
	{
		name = (char*)"Lemonade Stand";
		gold_sec = 1.2;
		price = 10;
		level = 0;
	}

	float get_Income()
	{
		return gold_sec * level;
	}

	void surprise(float& income)
	{
		float p = (float)rand() / RAND_MAX;
		if (p <= 0.1)
		{
			cout << "a dog stole your lemonade!!!" << endl;
			gold_sec *= 1.05;
			income -= 10;
		}
	}

	void raise_Level()
	{
		level++;
		cout << "a child screams" << endl;
	}
};

class Ice_Cream_Stand : public Shop
{
public:
	Ice_Cream_Stand()
	{
		name = (char*)"Ice Cream Stand";
		gold_sec = 5.4;
		price = 100;
		level = 0;
	}

	float get_Income()
	{
		return gold_sec + price * 0.1;
	}

	void surprise(float& income)
	{
		float p = (float)rand() / RAND_MAX;
		if (p <= level*0.1)
		{
			cout << "your efforts paid off!!!" << endl;
			gold_sec *= 1.01;
			income += 1;
		}
	}

	void raise_Level()
	{
		float p = (float)rand() / RAND_MAX;
		if (p <= 0.3)
		{
			level++;
			cout << "ice cream truck music plays" << endl;
		}
		else
		{
			cout << "GOOD LUCK NEXT TIME!" << endl;
		}
	}
};

class Hot_Dog_Stand : public Shop
{
public:
	Hot_Dog_Stand()
	{
		name = (char*)"Hot Dog Stand";
		gold_sec = 15;
		price = 500;
		level = 0;
	}

	float get_Income()
	{
		return gold_sec*level - price * 0.02;
	}

	void surprise(float& income)
	{
		float p = (float)rand() / RAND_MAX;
		if (p <= 0.05)
		{
			cout << "hot dog mutiny" << endl;
			gold_sec *= 0.9;
			level++;
		}
	}

	void raise_Level()
	{
		float p = (float)rand() / RAND_MAX;
		if (p <= 0.6)
		{
			price += 0.5;
			cout << "leave me alone" << endl;
		}
		else
		{
			cout << "GOOD LUCK NEXT TIME!" << endl;
		}
	}
};

int main()
{
	int n_shops = 0;
	Shop* shops[1000];
	float money = 15;

	for (;;)
	{
		cout << "you have " << money << " dollars." << endl;
		cout << "(b)uy or (s)kip : ";
		char c;
		cin >> c;
		if (c == 'b')
		{
			cout << "(l)emonade, (i)ce cream or (h)ot dog stand: ";
			char a;
			cin >> a;
			if (a == 'l')
			{
				Lemonade_Stand *p = new Lemonade_Stand();
				p->raise_Level();
				money -= 10;
				shops[n_shops++] = p;
			}
			else if (a == 'i')
			{
				Ice_Cream_Stand *p = new Ice_Cream_Stand();
				p->raise_Level();
				money -= 100;
				shops[n_shops++] = p;
			}
			else if (a == 'h')
			{
				Hot_Dog_Stand *p = new Hot_Dog_Stand();
				p->raise_Level();
				money -= 500;
				shops[n_shops++] = p;
			}
		}
		else if (c == 's')
		{
			for (int i = 0; i < n_shops; i++)
			{
				money += shops[i]->get_Income();
				shops[i]->surprise(money);
			}
		}
	}

	getchar();
	return 0;
}