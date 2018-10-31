#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>


void open_URL(char *url)
{
	char *path_to_executable = "start firefox -new-tab";

	static char command[1024];

	sprintf(command, "\"%s\" %s", path_to_executable, url);

	printf("command = %s\n", command);

	system(command);
}

int main()
{
	char *filename = "startup.txt";

	FILE *f = fopen(filename, "r");

	if (f == NULL)
	{
		printf("%s not found\n", filename);
		getchar();
		return 0;
	}

	int url_count = 0;
	for (;;)
	{
		static char url[4096];
		int r = fscanf(f, "%s", url);
		if (r != 1) break;

		open_URL(url);
		if(url_count == 0) Sleep(2000);
		else Sleep(100);

		url_count++;
	}

	
	printf("done\n");

	fclose(f);

	getchar();
	return 0;
}