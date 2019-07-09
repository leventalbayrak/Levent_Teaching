#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

struct Item
{
	char *name;
	float amount;
};

struct Recipe
{
	char *name;
	Item *ingredients;
	int n_ingredients;
};

void load_Single_Recipe(Recipe *recipe, const char* filename)
{
	FILE *f = fopen(filename, "r");

	recipe->name = (char*)malloc(sizeof(char) * 1001);
	fgets(recipe->name, 1000, f);
	int len = strlen(recipe->name);
	if (recipe->name[len - 1] == '\n') recipe->name[len - 1] = 0;

	recipe->ingredients = (Item *)malloc(sizeof(Item) * 1000);
	recipe->n_ingredients = 0;

	int count = 0;
	for (;;)
	{
		char amount[1001];
		if (fgets(amount, 1000, f) == NULL)
		{
			break;
		}
		recipe->ingredients[count].amount = atof(amount);
		
		recipe->ingredients[count].name = (char*)malloc(sizeof(char) * 1001);
		int len = strlen(recipe->ingredients[count].name);
		if (recipe->ingredients[count].name[len - 1] == '\n') recipe->ingredients[count].name[len - 1] = 0;
		if (fgets(recipe->ingredients[count].name, 1000, f) == NULL)
		{
			printf("FILE CORRUPTED\n");
			exit(0);
		}

		count++;
		
	}

	recipe->n_ingredients = count;

	printf("loaded %d ingredients\n", count);
}

void load_Recipes(Recipe *recipes, int *n_recipes, const char *filename)
{

	FILE *f = fopen(filename, "r");
	int count = 0;
	for (;;)
	{
		char line[1001];
		if (fgets(line, 1000, f) == NULL)
		{
			break;
		}

		int length = strlen(line);
		if(line[length - 1] == '\n') line[length - 1] = 0;
		printf("recipe filename %s\n", line);

		load_Single_Recipe(&recipes[count], line);

		count++;
		
	}

	*n_recipes = count;

	printf("loaded %d recipes\n", count);
}

void randomly_Pick_Recipes(Recipe *dest, const Recipe *src, int n_src, int n_dest)
{
	for (int i = 0; i < n_dest; i++)
	{
		int k = rand() % n_src;
		dest[i] = src[k];//shallow copy of pointers
	}
}

void generate_Item_List(Item *ingredients,int *n_ingredients, const Recipe *recipes, int n_recipes)
{
	int count = 0;
	for (int i = 0; i < n_recipes; i++)
	{
		for (int j = 0; j < recipes[i].n_ingredients; j++)
		{
			ingredients[count++] = recipes[i].ingredients[j];//shallow copy of name pointer
		}
	}

	*n_ingredients = count;
}

//will sum amount field
void get_Unique_Items(Item *unique, int *n_unique, const Item *src, int n_src)
{
	int k = 0;
	//for every item inside source
	for (int i = 0; i < n_src; i++)
	{
		//check if the item is already in the unique list
		bool found = false;
		for (int j = 0; j < k; j++)
		{
			if (strcmp(unique[j].name, src[i].name) == 0)
			{
				found = true;
				unique[j].amount += src[i].amount;
				break;
			}
		}

		if (!found)
		{
			unique[k++] = src[i];
		}

	}

	*n_unique = k;
}

void generate_HTML(const Item *groceries, int n_groceries,const Recipe *recipes, int n_recipes)
{
	FILE *f = fopen("groceries.html", "w");
	fprintf(f, "<html><body>");

	for (int i = 0; i < n_groceries; i++)
	{
		fprintf(f, "%s %.2f<br>", groceries[i].name, groceries[i].amount);
	}

	fprintf(f, "<h1><a href=\"dinners.html\">dinners</a></h1>");
	fprintf(f, "</html></body>");
	fclose(f);

	f = fopen("dinners.html", "w");
	fprintf(f, "<html><body>");

	for (int i = 0; i < n_recipes; i++)
	{
		fprintf(f, "<h1>%s</h1>", recipes[i].name);
		for (int j = 0; j < recipes[i].n_ingredients; j++)
		{
			fprintf(f, "%s %.2f<br>", recipes[i].ingredients[j].name, recipes[i].ingredients[j].amount);
		}
	}
	fprintf(f, "<h1><a href=\"groceries.html\">groceries</a></h1>");
	fprintf(f, "</html></body>");
	fclose(f);

}

int main()
{
	Recipe *recipes = (Recipe*)malloc(sizeof(Recipe) * 10000);
	
	int n_recipes = 0;
	load_Recipes(recipes, &n_recipes, "recipelist.txt");

	Recipe *weekly_recipes = (Recipe*)malloc(sizeof(Recipe) * 10000);
	int n_weekly_recipes = 7;
	randomly_Pick_Recipes(weekly_recipes, recipes, n_recipes, n_weekly_recipes);

	Item *all_ingredients = (Item *)malloc(sizeof(Item) * 10000000);
	int n_all_ingredients = 0;
	generate_Item_List(all_ingredients, &n_all_ingredients, weekly_recipes, n_weekly_recipes);

	Item *unique_ingredients = (Item *)malloc(sizeof(Item) * 10000000);
	int n_unique_ingredients = 0;
	get_Unique_Items(unique_ingredients, &n_unique_ingredients, all_ingredients, n_all_ingredients);

	generate_HTML(unique_ingredients, n_unique_ingredients, weekly_recipes, n_weekly_recipes);

	return 0;
}