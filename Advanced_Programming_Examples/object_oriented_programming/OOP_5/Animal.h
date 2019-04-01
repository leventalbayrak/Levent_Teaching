#pragma once

class Animal
{
	int energy;

	void digest(int e);

protected:
	void eat(int e);

public:
	Animal();

	~Animal();

	void hunt();

};
