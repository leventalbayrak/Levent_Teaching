#include <iostream>
using namespace std;

#include "Hash_Example.h"

int main()
{

	Key_Value_Pair::Key_Value_Pair h;
	Key_Value_Pair::init(&h, 10007);

	unsigned long long apple_key = Encryption::encrypt_djb2("apple_key", strlen("apple_key"));
	unsigned long long orange_key = Encryption::encrypt_djb2("orange_key", strlen("orange_key"));

	Key_Value_Pair::set(&h, apple_key, 2.62);
	Key_Value_Pair::set(&h, orange_key, 3.14);

	cout << Key_Value_Pair::get(&h, apple_key) << endl;
	cout << Key_Value_Pair::get(&h, orange_key) << endl;
	
	getchar();
	return 0;
}

