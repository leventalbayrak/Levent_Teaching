#include <stdio.h>
#pragma warning(disable:4996)

int main()
{

	int data[5] = { 1,2,3,4,55 };

	FILE *f1 = fopen("f1.txt", "w");
	for (int i = 0; i < 5; i++)
	{
		fprintf(f1, "%d\n", data[i]);
	}
	fclose(f1);

	FILE *f2 = fopen("f2.txt", "wb");
	for (int i = 0; i < 5; i++)
	{
		fprintf(f2, "%d", data[i]);
	}
	fclose(f2);

	FILE *f3 = fopen("f3.txt", "w");
	fwrite(data, sizeof(int), 5, f3);
	fclose(f3);

	FILE *f4 = fopen("f4.txt", "wb");
	fwrite(data, sizeof(int), 5, f4);
	fclose(f4);

	return 0;
}