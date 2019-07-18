#include <iostream>
using namespace std;
#pragma warning(disable:4996)

class Person
{
protected:
	char *name;
	int SSN;

public:
	Person()
	{
		name = (char*)"N/A";
		SSN = 0;
	}

	void set_SSN(int ssn)
	{
		SSN = ssn;
	}

	virtual void print() = 0;
};

class Student : public Person
{
	float GPA;
	int id;
public:
	Student(int _id)
	{
		id = _id;
		GPA = 0.0;
	}

	void set_GPA(float _gpa)
	{
		GPA = _gpa;
	}

	void print()
	{
		cout << "student: " << name << endl;
		cout << "gpa: " << GPA << endl;
	}

	virtual void walk()
	{
		cout << "walking to class" << endl;
	}
};

class Employee : public Person
{
	int id;
	char job_title[20];
	float salary;
public:
	void print()
	{
		cout << "employee: " << name << endl;
		cout << "id: " << id << endl;
		cout << "title: " << job_title << endl;
	}

	virtual void talk()
	{
		cout << "i am introducing myself" << endl;
	}
};

class Staff : public Employee
{
	float pay_rate;
	int start_time;
	int end_time;
public:
	void print()
	{
		cout << "employee(staff): " << name << endl;
		cout << "id: " << pay_rate << endl;
		cout << "start: " << start_time << endl;
		cout << "end: " << end_time << endl;
	}
	virtual void talk()
	{
		cout << "I am Mark, I am introducing myself" << endl;
	}
};

class Teacher : public Employee
{
	float pay_rate;
	int classes[20];
	int number_of_markers;
public:
	void print()
	{
		cout << "employee(teacher): " << name << endl;
		cout << "id: " << pay_rate << endl;
		cout << "number_of_markers: " << number_of_markers << endl;
	}
	virtual void talk()
	{
		cout << "I am NOT Mark, I am introducing myself" << endl;
	}
};

int main()
{

	Person *people[4] = { &Student(1233),&Employee(), &Staff(), &Teacher() };
	for (int i = 0; i < 4; i++)
	{
		people[i]->print();
	}
	getchar();
	return 0;
}