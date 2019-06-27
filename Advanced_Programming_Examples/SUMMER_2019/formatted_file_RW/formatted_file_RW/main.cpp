#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

void write_Formatted_Example(FILE *f)
{
	
	//the format we want to write is:
	/*
	<?xml version="1.0" encoding="UTF-8"?>
	<map width="20" height="20" infinite="0" name="Mark">
	*/
	//you want to be able to change the 'name', 'width' and 'height'

	const char *first_line_template = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n";
	const char *second_line_template = "<map width = \"%d\" height = \"%d\" infinite = \"0\" name = \"%s\">\n";

	fprintf(f, first_line_template);
	fprintf(f, second_line_template, 15, 16, "Dota");

}

void write_Numbers(FILE *f)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			fprintf(f, "%d,", rand() % 100);//make sure you dont write a comma at the end
		}
		fprintf(f, "\n");
	}
}

void read_Formatted_Example(FILE *f)
{
	//we want to read 'width','height' and 'name'
	/*
	<map width = "15" height = "16" infinite = "0" name = "Dota">
	*/

	char line[1025];
	fgets(line, 1024, f);
	printf("%s\n", line);
	char width[20];
	char height[20];
	char name[20];

	sscanf(line, "<map width = \"%[^\"]\" height = \"%[^\"]\" infinite = \"0\" name = \"%[^\"]\">", width, height, name);

	//printf("%s %s %s\n", width, height, name);

	int w = atoi(width);
	int h = atoi(height);

	//printf("final result: %d %d %s\n", w, h, name);

	//BETTER METHOD

	printf("result\n");
	sscanf(line, "%*[^w]width = \"%[^\"]%*[^h]height = \"%[^\"]%*[^a]ame = \"%[^\"]", width, height, name);

	printf("%s %s %s\n", width, height, name);

	w = atoi(width);
	h = atoi(height);

	printf("final result: %d %d %s\n", w, h, name);

}

int main()
{
	FILE *f_out = fopen("example_out.txt", "w");
	write_Formatted_Example(f_out);
	write_Numbers(f_out);
	fclose(f_out);

	FILE *f_in = fopen("example_in.txt", "r");
	read_Formatted_Example(f_in);
	//read_Formatted_Example(f_in);

	getchar();
	return 0;
}