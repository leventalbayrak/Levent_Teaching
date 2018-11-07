#pragma warning(disable:4996)
#include <stdio.h>

//there are much more efficient ways to do this task! google it
int string_Length(const char *str)
{
	int length = 0;
	for (;;)
	{
		if (str[length] == 0) break;
		length++;
	}

	return length;
}

int is_Palindrome(const char *str)
{
	int length = string_Length(str);
	int number_of_steps = length / 2;
	
	for (int i = 0; i < number_of_steps; i++)
	{
		int left = i;
		int right = length - i - 1;

		if (str[left] != str[right]) return 0;
	}

	return 1;
}

int main()
{
	const char *word = "racecar";

	int result = is_Palindrome(word);

	if (result == 1)
	{
		printf("%s is a palindrome!\n", word);
	}
	else
	{
		printf("%s is NOT a palindrome!\n", word);
	}

	getchar();
	return 0;
}