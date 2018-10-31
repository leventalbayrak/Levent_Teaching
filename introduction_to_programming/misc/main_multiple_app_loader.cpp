#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>

void unknown(char *app)
{
	static char command[1024];
	sprintf(command, "start %s", app);

	printf("command = %s\n", command);
	system(command);
	Sleep(100);
}

void firefox(char *url)
{
	static char command[1024];
	static int number_of_tabs_opened = 0;

	char *command_prefix = "start firefox -new-tab";

	sprintf(command, "\"%s\" %s", command_prefix, url);

	printf("command = %s\n", command);

	system(command);
	if (number_of_tabs_opened) Sleep(1000);
	else Sleep(100);
	
	number_of_tabs_opened++;
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

	int app_count = 0;
	for (;;)
	{
		static char app[256];
		int r = fscanf(f, "APP %s\n", app);
		if (r != 1) break;

		if (strcmp(app, "firefox") == 0)
		{
			static char url[4096];
			int r = fscanf(f, "%s\n", url);
			if (r != 1) break;

			firefox(url);
		}
		else
		{
			unknown(app);
		}
		
		app_count++;
	}

	
	printf("done\n");

	fclose(f);

	getchar();
	return 0;
}