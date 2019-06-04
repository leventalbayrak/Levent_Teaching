
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *p = (int*)malloc(sizeof(int) * 4); //dynamic heap allocation from operating system

	*p = 1234;
	*(p + 1) = 4567;

	printf("p=%u p+1=%u &p=%u *p=%d *(p+1)=%d\n", p, p+1, &p, *p, *(p + 1));

	p[0] = 4321; //same thing as *(p+0) ==> *p
	p[1] = 7654; //same thing as *(p + 1)
	printf("p=%u p+1=%u &p=%u *p=%d *(p+1)=%d\n", p, p + 1, &p, *p, *(p + 1));

	int s[4] = { 0 }; //dynamic allocation on stack
	printf("s=%u s+1=%u &s=%u *s=%d *(s+1)=%d\n", s, s + 1, &s, *s, *(s + 1));
	//p=s; <=== valid
	//s=p; <=== invalid
	printf("s=%u s+1=%u &s=%u s[0]=%d s[1]=%d\n", s, s + 1, &s, s[0], s[1]);

	int g[4] = {1,2,3,4}; //dynamic allocation on stack
	printf("g=%u g+1=%u &g=%u g[0]=%d g[1]=%d\n", g, g + 1, &g, g[0], g[1]);

	int wow[4][4] = { 0 };
	wow[0][0] = 3;

	int **f = (int**)malloc(sizeof(int*) * 4);
	f[0] = (int*)malloc(sizeof(int) * 4);
	f[1] = (int*)malloc(sizeof(int) * 4);
	f[2] = (int*)malloc(sizeof(int) * 4);
	f[3] = (int*)malloc(sizeof(int) * 4);

	f[0][0] = 4;

	free(p);

	free(f[0]);
	free(f[1]);
	free(f[2]);
	free(f[3]);

	free(f);

	for (int i = 0; i < 100000; i++)
	{
		int *p = (int*)malloc(sizeof(int) * 1000000);
		printf("%d\n", p[0]);
		
	}

	getchar();
	return 0;
}