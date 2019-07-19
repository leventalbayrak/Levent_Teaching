#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

enum {
	SEEK_RECIPE, DUMP_RECIPE,
	DUMP_INGREDIENTS, DUMP_METHOD
};

int main()
{
	FILE *f = fopen("recipe.txt", "r");
	
	char line[1025];

	char *recipe_name = (char*)malloc(sizeof(char) * 10000);
	int recipe_name_length = 0;

	char *method = (char*)malloc(sizeof(char) * 100000);
	int method_length = 0;

	int state = SEEK_RECIPE;
	for (;;)
	{
		char *r = fgets(line, 1024, f);

		if (state == SEEK_RECIPE)
		{
			
			if (r == NULL)
			{
				printf("file corrupted!\n");
				exit(0);
			}
			//somewhere here, you need to make
			//state = DUMP_RECIPE
		
			if (strcmp(line, "recipe:\n") == 0)
			{
				state = DUMP_RECIPE;
			}

		}
		else if (state == DUMP_RECIPE)
		{
			if (r == NULL)
			{
				printf("file corrupted while dumping recipe!\n");
				exit(0);
			}

			if (strcmp(line, "ingredients:\n") == 0)
			{
				recipe_name[recipe_name_length] = 0;
				printf("THIS IS A RECIPE:\n%s\n", recipe_name);
				recipe_name_length = 0;
				state = DUMP_INGREDIENTS;
				continue;
			}

			//keep dumping recipe name somewhere
			int len = strlen(line);
			for (int i = 0; i < len; i++)
			{
				recipe_name[recipe_name_length++] = line[i];
			}

			
		}
		else if (state == DUMP_INGREDIENTS)
		{
			if (r == NULL)
			{
				printf("file corrupted while dumping ingredients!\n");
				exit(0);
			}

			//somewhere here, you need to make
			//state = DUMP_METHOD

			if (strcmp(line, "method:\n") == 0)
			{
				state = DUMP_METHOD;
			}
		}
		else if (state == DUMP_METHOD)
		{
			if (r == NULL)
			{
				method[method_length] = 0;
				printf("%s\n", method);
				method_length = 0;
				printf("done reading file!\n");
				break;
			}

			//somewhere here, you need to make
			//state = DUMP_RECIPE

			if (strcmp(line, "recipe:\n") == 0)
			{
				method[method_length] = 0;
				state = DUMP_RECIPE;

				printf("THIS IS METHOD:\n%s\n", method);
				method_length = 0;
				continue;
			}

			//keep dumping method somewhere
			int len = strlen(line);
			for (int i = 0; i < len; i++)
			{
				method[method_length++] = line[i];
			}

		
		}
	}


	//now you read the file
	//do other stuff

	getchar();
	return 0;
}