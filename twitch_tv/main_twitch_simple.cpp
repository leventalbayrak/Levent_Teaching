#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Twitch.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
	//initialize network
	Twitch::startup();

	//init user name, oauth
	Twitch::Connection connection;
	Twitch::init(&connection, "plogp", "8rqzvhzsaplxo858wheg3n7hogo09b");
	
	//make TCP connection to twitch and login
	Twitch::connect(&connection);

	//join a channel
	Twitch::join_Channel(&connection, "sodapoppin");
	
	//join another channel
	Twitch::join_Channel(&connection, "destiny");

	//incoming message list from all connected channels
	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	FILE *f_log = fopen("log.txt", "w+"); assert(f_log);

	printf("chat log\n");
	for(;;)
	{
		unsigned int timestamp = clock();
		
		//collect all messages from all channels
		Twitch::communicate(&incoming, &connection, timestamp);
		if (connection.active == false)
		{
			printf("connection was closed!\n");
			break;
		}

		//print received messages from all channels
		for (int i = 0; i < incoming.n_count; i++)
		{
			fprintf(f_log, "%s@%s|(%.2f)-> %s\n",incoming.username[i], incoming.channel[i], (double)timestamp / CLOCKS_PER_SEC, incoming.message[i]);
		}
		

	}
	
	fclose(f_log);

	getchar();
}