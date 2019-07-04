#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

struct Gear
{
	int id;
	int rarity;
	int type;
	int dmg;
	int resistance;
	float cost;
};

struct Champion
{
	char *name;
	int hp;
	int damage;

	Gear *gear;
	int n_gear;
};

/*
From a database of champions, randomly select N
*/
/*
look up Durstenfeld shuffle
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
*/
void generate_Subset(Champion *dest,int n_dest,const Champion *src, int n_src)
{
	for (int i = 0; i < n_dest; i++)
	{
		int k = rand() % n_src;
		dest[i] = src[k];//direct copy of every field
		//warning: name is just a pointer! 
		//if you modify dest[i].name, src[k].name will be modified
	}
}

void generate_Gear_List(Gear *dest,int *n_dest, const Champion *src, int n_src)
{
	int k = 0;
	for (int i = 0; i < n_src; i++)
	{
		//ith champion
		for (int j = 0; j < src[i].n_gear; j++)
		{
			//jth gear of champion i
			dest[k++] = src[i].gear[j];
		}
	}
	*n_dest = k;
}

void get_Unique_Gear_List_Cost(Gear *dest, int *n_dest, const Gear *src, int n_src)
{
	int k = 0;
	for (int i = 0; i < n_src; i++)
	{
		//check if this gear is already in destination

		bool found = false;
		for (int j = 0; j < k; j++)
		{
			if (dest[j].id == src[i].id)
			{
				//item already exists
				dest[j].cost += src[i].cost;
				found = true;
				break;
			}
		}

		if (found == false)
		{
			dest[k++] = src[i];
		}

	}
}

int main()
{

	return 0;
}