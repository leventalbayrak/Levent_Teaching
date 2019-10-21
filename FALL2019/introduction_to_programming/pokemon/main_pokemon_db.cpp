#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

struct Pokemon
{
	char name[64];
	char type[64];
	char legendary[8];//TRUE FALSE
	int id;
	int total;
	int hp;
	int attack;
	int defense;
	int sp_attack;
	int sp_defense;
	int speed;
	int generation;	
};

void print_Summary(Pokemon* p)
{
	printf("%s\t%s\t%d\n", p->name, p->type, p->total);
}

void print_Full(Pokemon* p)
{
	printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
		p->id,
		p->name,
		p->type,
		p->total,
		p->hp,
		p->attack,
		p->defense,
		p->sp_attack,
		p->sp_defense,
		p->speed,
		p->generation,
		p->legendary
	);
}

int load_Database(Pokemon*db, const char* filename)
{
	int number_of_pokemon = 0;
	FILE* f = fopen(filename, "r");
	for (;;)
	{
		/*
		#	Name	Type 1	Total	HP	Attack	Defense	Sp. Atk	Sp. Def	Speed	Generation	Legendary
		*/

		int number_of_items_read = fscanf(f, "%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s",
			&db[number_of_pokemon].id,
			db[number_of_pokemon].name,
			db[number_of_pokemon].type,
			&db[number_of_pokemon].total,
			&db[number_of_pokemon].hp,
			&db[number_of_pokemon].attack,
			&db[number_of_pokemon].defense,
			&db[number_of_pokemon].sp_attack,
			&db[number_of_pokemon].sp_defense,
			&db[number_of_pokemon].speed,
			&db[number_of_pokemon].generation,
			db[number_of_pokemon].legendary
		);

		if (number_of_items_read == 12)
		{
			number_of_pokemon++;
		}
		else if (number_of_items_read == EOF)//EOF is -1
		{
			break;
		}
		else
		{
			printf("ERROR reading file %d\n", number_of_items_read);
			getchar();
			exit(0);
		}

	}

	return number_of_pokemon;
}

void fake_Dots()
{
	_sleep(200);
	printf(".");
	_sleep(300);
	printf(".");
	_sleep(400);
	printf(".\n");
}

int main()
{
	static Pokemon database[100000];

	/*
	LOAD FILE INTO ARRAY
	*/
	int number_of_pokemon = load_Database(database, "pokemon_db.txt");

	/*
	database interface
	*/
	for (;;)
	{
		printf("PKMN[DB]>");
		
		char type[32];
		scanf("%s", type);
		printf("searching extended Pokemon database\n");
		fake_Dots();

		int found_counter = 0;
		for (int i = 0; i < number_of_pokemon; ++i)
		{

			if (rand() % 2 == 0)
			{
				_sleep(10);
			}

			if (strcmp(database[i].type, type) == 0)
			{
				
				print_Summary(&database[i]);
				found_counter++;
			}
		}
		printf("found %d entries\n", found_counter);
	}


	printf("discovered %d pokemon entries!\n", number_of_pokemon);
	printf("done!\n");
	getchar();
	return 0;
}