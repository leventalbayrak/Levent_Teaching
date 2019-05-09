//https://www.hackerrank.com/challenges/balanced-brackets/problem
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve(char *brackets, int n_brackets)
{

	return 0;
}

int main()
{
	//hackerrank
	//FILE *f_in = stdin;
	//FILE *f_out = fopen(getenv("OUTPUT_PATH"), "w");

	FILE *f_in = fopen("testcase_custom.txt", "r");
	FILE *f_out = stdout;

	int n_test_cases = 0;
	fscanf(f_in, "%d", &n_test_cases);
	for (int i = 0; i < n_test_cases; i++)
	{
		char brackets[1001];
		fscanf(f_in, "%s", brackets);
		int len = strlen(brackets);

		//solve problem here
		int r = solve(brackets, len);
		if (r == 0)
		{
			fprintf(f_out, "NO\n");
		}
		else
		{
			fprintf(f_out, "YES\n");
		}
	}


	getchar();
	return 0;
}


