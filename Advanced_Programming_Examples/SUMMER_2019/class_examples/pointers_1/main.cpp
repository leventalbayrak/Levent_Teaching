
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int x = 5;
	char y = 'A';
	char y_not = 'A';
	char but_y = 'A';
	char but_y_not = 'A';
	char idk = 'A';
	short donut = 1;

	printf("x=%d &x=%u\n", x, &x);
	printf("y=%c &y=%u\n", y, &y);
	printf("y_not=%c &y_not=%u\n", y_not, &y_not);
	printf("but_y=%c &but_y=%u\n", but_y, &but_y);
	printf("but_y_not=%c &but_y_not=%u\n", but_y_not, &but_y_not);
	printf("idk=%c &idk=%u\n", idk, &idk);
	printf("donut=%d &donut=%u\n", donut, &donut);

	int *p = &x;
	printf("p=%u &p=%u *p=%d\n", p, &p, *p);

	char *s = &y;
	printf("s=%u &s=%u *s=%c\n", s, &s, *s);

	getchar();
	return 0;
}