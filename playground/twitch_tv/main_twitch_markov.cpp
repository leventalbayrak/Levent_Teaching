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

	FILE *f_chat = fopen("chat.txt", "w+");
	FILE *f_gen = fopen("gen.txt", "w+");
	
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

	static const char *username = "plogp";
	static const char *token = "zi5igvfgn3914hg3hczbe497m8uzpp";

	Twitch::Connection connection;
	Twitch::init(&connection, username, token);
	
	Twitch::connect(&connection);

	Twitch::join_Channel(&connection, "ninja");
	Twitch::join_Channel(&connection, "timthetatman");
	Twitch::join_Channel(&connection, "highdistortion");
	Twitch::join_Channel(&connection, "solaryfortnite");
	Twitch::join_Channel(&connection, "chap");
	Twitch::join_Channel(&connection, "robi");

	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	unsigned int last_n_generated = 0;
	unsigned int max_generated = 200000;
	unsigned int n_generated = 0;

	printf("chat log\n");
	for (;;)
	{
		if (n_generated >= max_generated) break;
	
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
			Generator::add_Str(&g, (unsigned char*)incoming.message[i], strlen(incoming.message[i]));
			printf("add_Str: len %d\n", strlen(incoming.message[i]));

			fprintf(f_chat, "%s\t%s\n",incoming.channel[i], incoming.message[i]);
			
			last_n_generated++;
		}
		
		if (last_n_generated >= 1)
		{
			last_n_generated = 0;
			static char tmp[1024];
			tmp[0] = 0;
			Generator::generate((unsigned char*)tmp,1023, &g, g.nmer_size);
			printf("generate: len %d\n", strlen(tmp));
			fprintf(f_gen, "%u\t%u\t%u\t%s\n",n_generated,g.root.size,g.root.sum, tmp);
			n_generated++;
			printf("%u\n", n_generated);
		}
		
		
	}
	fclose(f_chat);
	fclose(f_gen);
	getchar();
}