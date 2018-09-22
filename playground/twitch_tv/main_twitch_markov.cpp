#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../../include/twitch_tv/Twitch_core.h"
#include "../../include/word_gen/Word_Gen.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
	int nmer_size = 4;
	
	Generator::Generator g;
	Generator::init(&g, nmer_size);

	////static makes problem go away
	////there is an overflow
	//char t[1024];
	//for (;;)
	//{
	//	int len = rand() % 1000;
	//	for (int i = 0; i < len; i++)
	//	{
	//		t[i] = 'a' + rand() % 26;
	//	}
	//	t[len] = 0;

	//	Generator::add_Str(&g, t, len);
	//	
	//	Generator::generate(t, 1023, &g, g.nmer_size);

	//	printf("len %d %u\n",len, g.root.sum);
	//}
	//getchar();
	Twitch::startup();

	const char *username = "plogp";
	const char *token = "zi5igvfgn3914hg3hczbe497m8uzpp";


	Twitch::Connection connection;
	Twitch::init(&connection, username, token);
	
	Twitch::connect(&connection);

	Twitch::join_Channel(&connection, "aphromoo");
	Twitch::join_Channel(&connection, "itshafu");
	Twitch::join_Channel(&connection, "pikabooirl");
	Twitch::join_Channel(&connection, "agony");
	Twitch::join_Channel(&connection, "yelo"); 
	Twitch::join_Channel(&connection, "voyboy");

	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	unsigned int last_n_generated = 0;

	printf("chat log\n");
	for(;;)
	{
		unsigned int timestamp = clock();
		
		Twitch::Message::clear(&incoming);
		Twitch::communicate(&incoming, &connection, timestamp);
		if (connection.active == false)
		{
			printf("connection was closed!\n");
			break;
		}

		for (int i = 0; i < incoming.n_count; i++)
		{
			Generator::add_Str(&g, incoming.message[i], strlen(incoming.message[i]));
			//Generator::add_Str(&g, incoming.username[i]);

			last_n_generated++;
		}
		
		if (last_n_generated >= 1)
		{
			last_n_generated = 0;
			static char tmp[1024];
			tmp[0] = 0;
			Generator::generate(tmp,1023, &g, g.nmer_size);
			printf("%s\n", tmp);
		}
		
		
	}
	
	getchar();
}