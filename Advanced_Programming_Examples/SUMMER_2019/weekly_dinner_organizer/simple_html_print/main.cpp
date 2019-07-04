#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//SILLY RECIPE GENERATOR 

const char *ingredients[] = { 
	"apple", "chicken", "beef",
	"potato", "cheese" 
};
const char *knife_actions[] = { 
	"cut", "chop", "dice", 
	"grate", "tenderize" 
};
const char *prepare_actions[] = { 
	"marinate", "mix", "stir", 
	"pour", "blend", "roll", 
	"knead"
};
const char *cooking_actions[] = { 
	"bake", "fry", "grill",
	"boil", "sear" 
};
const char *adjectives[] = {
	"small", "big", "medium",
	"rare", "well done", "red",
	"raw", "pittsburgh" 
};

void make_Recipe(const char *filename, const char *links_to)
{
	int n_ingredients = 5;
	int n_knife_actions = 5;
	int n_prepare_actions = 7;
	int n_cooking_actions = 5;
	int n_adjectives = 8;

	FILE *f = fopen(filename, "w");
	fprintf(f, "<HTML><HEAD><p>EAT THIS!</p></HEAD>");

	fprintf(f, "<BODY>");
	//everything goes here
	int n_sentences = 3;
	for (int i = 0; i < n_sentences; i++)
	{
		int a = rand() % n_adjectives;
		int b = rand() % n_ingredients;
		int b2 = rand() % n_ingredients;
		int c = rand() % n_knife_actions;
		int d = rand() % n_cooking_actions;
		int e = rand() % n_prepare_actions;
		fprintf(f, "<p>");
		fprintf(f, "take %s %s, %s it.", adjectives[a], ingredients[b], knife_actions[c]);
		fprintf(f, "</p>");

		fprintf(f, "<p>");
		fprintf(f, "%s the %s", prepare_actions[e], ingredients[b]);
		fprintf(f, "</p>");

		fprintf(f, "<p>");
		fprintf(f, "%s the %s with %s", cooking_actions[d], ingredients[b], ingredients[b2]);
		fprintf(f, "</p>");
	}


	fprintf(f, "<a href=\"https://www.youtube.com/watch?v=dLvoNRiaasM\" > click to see youtube link </a>");

	if (links_to != NULL)
	{
		fprintf(f, "<br>");
		fprintf(f, "<a href=\"%s\" > HERE IS SOME SALAD RECIPE TO GO WITH IT! </a>", links_to);
	}

	fprintf(f, "</BODY>");
	fprintf(f, "</HTML>");

	fclose(f);
}

int main()
{
	//POINTER AND ARRAY LIFE LESSON
	int *p = (int*)malloc(sizeof(int) * 1000);
	printf("p: %d adjectives: %d\n",sizeof(p), sizeof(adjectives));

	make_Recipe("index.html", "another.html");
	make_Recipe("another.html", NULL);


	getchar();
	return 0;
}