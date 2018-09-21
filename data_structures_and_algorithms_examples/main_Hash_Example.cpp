#include <iostream>
using namespace std;

#include "Hash_Example.h"

int main()
{

	Key_Value_Pair::Key_Value_Pair h;
	Key_Value_Pair::init(&h, 10007);

	Key_Value_Pair::set(&h, 10007, 120);
	Key_Value_Pair::set(&h, 10007 * 2, 130);

	cout << Key_Value_Pair::get(&h, 10007) << endl;
	cout << Key_Value_Pair::get(&h, 10007 * 2) << endl;
	
	getchar();
	return 0;
}

