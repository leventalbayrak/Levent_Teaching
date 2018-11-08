#pragma once

#include "String_core.h"

namespace Str_Ops
{
	int is_Palindrome(const char *str)
	{
		int k = length(str);

		if (k <= 1) return 0;

		int number_of_steps = k / 2;

		for (int i = 0; i < number_of_steps; i++)
		{
			int left = i;
			int right = k - i - 1;

			if (str[left] != str[right]) return 0;
		}

		return 1;
	}
}
