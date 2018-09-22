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

	Twitch::startup();

	const char *username = "plogp";
	const char *token = "zi5igvfgn3914hg3hczbe497m8uzpp";
	const char *channel = "aphromoo";

	Twitch::Connection connection;
	Twitch::init(&connection, username, token);
	
	Twitch::connect(&connection);

	Twitch::join_Channel(&connection, channel);

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
			Generator::add_Str(&g, incoming.message[i]);
			Generator::add_Str(&g, incoming.username[i]);

			last_n_generated++;
		}
		
		if (last_n_generated >= 1)
		{
			last_n_generated = 0;
			static char tmp[1024];
			Generator::generate(tmp, &g);
			printf("%s\n", tmp);
		}
		
		
	}
	
	getchar();
}