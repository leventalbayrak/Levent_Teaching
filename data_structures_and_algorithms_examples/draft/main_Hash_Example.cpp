/*
Data structures and algorithms
Levent Albayrak 2018
*/

#pragma warning(disable:4996)
#include <iostream>
using namespace std;
#include <time.h>
#include <Windows.h>
#include "Hash_Example.h"

int main()
{
	srand(time(0));
	//Assume we want to associate game titles with a rating and a description
	
	//we will pass the game titles to our hash functions to generate keys
	//rest of the data (rating, description and developer) is for querying
	//Additionally, we will associate developer names with websites, here is the data
	
	
	//Let's store the game ratings in the Open Hash implementation which can store doubles
	//this implementation uses modular indexing, so array size will be a prime number
	//initializing the table
	Open_Hash_Modular_Indexing::Table rating_table;
	Open_Hash_Modular_Indexing::init(&rating_table, 10007);

	//Let's store descriptions in the Closed Hash implementation which can store pointers (any pointer because it is void*)
	//this implementation uses modular indexing, so array size will be a prime number
	//initializing the table
	Closed_Hash_Linear_Probe_Modular_Indexing::Table description_table;
	Closed_Hash_Linear_Probe_Modular_Indexing::init(&description_table, 10007);

	//Finally, we will store the developer name in the other Closed Hash implementation which uses multiplicative indexing
	//we need to pass size_bits to the init function rather than the actual size
	//the array size will be 2^size_bits
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::Table developer_table;
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::init(&developer_table, 14);//table size will be 2^14 = 16384

	Closed_Hash_Linear_Probe_Multiplicative_Indexing::Table developer_website_table;
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::init(&developer_website_table, 14);//table size will be 2^14 = 16384


	//we begin with associating developers with the websites
	//there are 3 developers
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
			&developer_website_table,//the table
			Encryption::encrypt_djb2("BLIZZARD", strlen("BLIZZARD")),//convert developer name to a key
			"www.blizzard.com"//associate with the website string
		);
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
		&developer_website_table,//the table
		Encryption::encrypt_djb2("Riot Games", strlen("Riot Games")),//convert developer name to a key
		"www.riotgames.com"//associate with the website string
	);
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
		&developer_website_table,//the table
		Encryption::encrypt_djb2("Psyonix", strlen("Psyonix")),//convert developer name to a key
		"www.psyonix.com"//associate with the website string
	);
	
	
	//associate game title with the rating, description and developer name
	//there are 4 titles
	//associate titles to ratings
	Open_Hash_Modular_Indexing::set(
			&rating_table,
			Encryption::encrypt_djb2("World of Warcraft", strlen("World of Warcraft")),//convert game title to a key
			95.2//give it a rating
		);
	Open_Hash_Modular_Indexing::set(
		&rating_table,
		Encryption::encrypt_djb2("League of Legends", strlen("League of Legends")),//convert game title to a key
		91.05//give it a rating
	);
	Open_Hash_Modular_Indexing::set(
		&rating_table,
		Encryption::encrypt_djb2("Rocket League", strlen("Rocket League")),//convert game title to a key
		93.6//give it a rating
	);
	Open_Hash_Modular_Indexing::set(
		&rating_table,
		Encryption::encrypt_djb2("Overwatch", strlen("Overwatch")),//convert game title to a key
		92.7//give it a rating
	);
	
	//there are 4 descriptions
	//associate titles to descriptions
	Closed_Hash_Linear_Probe_Modular_Indexing::set(
			&description_table,
			Encryption::encrypt_djb2("World of Warcraft", strlen("World of Warcraft")),//convert game title to a key
			"the greatest MMORPG ever made"
		);
	Closed_Hash_Linear_Probe_Modular_Indexing::set(
		&description_table,
		Encryption::encrypt_djb2("League of Legends", strlen("League of Legends")),//convert game title to a key
		"the most popular competitive MOBA game"
	);
	Closed_Hash_Linear_Probe_Modular_Indexing::set(
		&description_table,
		Encryption::encrypt_djb2("Rocket League", strlen("Rocket League")),//convert game title to a key
		"the highest skill cap in the world"
	);
	Closed_Hash_Linear_Probe_Modular_Indexing::set(
		&description_table,
		Encryption::encrypt_djb2("Overwatch", strlen("Overwatch")),//convert game title to a key
		"higher quality Team Fortress without the hats"
	);

	//populate the title developer table
	//world of warcraft is made by blizzard
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
		&developer_table,
		Encryption::encrypt_djb2("World of Warcraft", strlen("World of Warcraft")),//convert game title to a key
		"BLIZZARD"
	);
	//league of legends is made by riot games
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
		&developer_table,
		Encryption::encrypt_djb2("League of Legends", strlen("League of Legends")),//convert game title to a key
		"Riot Games"
	);
	//rocket league is made by psyonix
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
		&developer_table,
		Encryption::encrypt_djb2("Rocket League", strlen("Rocket League")),//convert game title to a key
		"Psyonix"
	);
	//overwatch is also made by blizzard
	Closed_Hash_Linear_Probe_Multiplicative_Indexing::set(
		&developer_table,
		Encryption::encrypt_djb2("Overwatch", strlen("Overwatch")),//convert game title to a key
		"BLIZZARD"
	);



	for (;;)
	{

		cout << endl;
		cout << "press any key to look-up a game" << endl;
		getchar();

		char *game_title[4] = { "World of Warcraft", "League of Legends", "Rocket League", "Overwatch" };
		int k = rand() % 4;//pick a random title

		//convert it into a key
		unsigned long long title_key = Encryption::encrypt_djb2(game_title[k], strlen(game_title[k]));
		//get its rating, developer name and description using the key
		double rating = Open_Hash_Modular_Indexing::get(&rating_table, title_key);
		char *description = (char*)Closed_Hash_Linear_Probe_Modular_Indexing::get(&description_table, title_key);//you must explicitly cast void* into char*
		char *developer = (char*)Closed_Hash_Linear_Probe_Multiplicative_Indexing::get(&developer_table, title_key);//you must explicitly cast void* into char*
		
		//developer names are associated to websites
		//generate the developer key from the developer name that you obtained from the previous look-up
		unsigned long long dev_key = Encryption::encrypt_djb2(developer, strlen(developer));
		//use the key to look up the website
		char *website = (char*)Closed_Hash_Linear_Probe_Multiplicative_Indexing::get(&developer_website_table, dev_key);

		//print out the results
		cout << "Game title: " << game_title[k] << endl;
		//pretend that it takes time
		Sleep(500);

		cout << "Developer: " << developer << endl;
		cout << "Rating: " << rating << endl;
		cout << "Website: " << website << endl;
		cout << "Description: " << description << endl;
	}



	
	return 0;
}

