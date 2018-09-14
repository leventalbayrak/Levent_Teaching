#pragma once

#include "Simple_TCP.h"
#include "Twitch_data.h"
#include "Twitch_Message_core.h"

#include <time.h>
#include <iostream>
#include <fstream>
#include <assert.h>
using namespace std;

namespace Twitch
{

	//call once to initialize network
	void startup();

	//initialize connection data
	void init(Connection *c, const char *username, const char *oauth);

	//close active socket
	void close(Connection *c);

	//creates a socket and makes a TCP connection
	//shutsdown connection if it already exists and reconnects
	void connect(Connection *c);

	//update tcp buffer, responds to pings immediately
	//returns number of bytes read from socket
	int update(Connection *c);

	//joins a channel
	//static string read/write inside
	void join_Channel(Connection *c, const char *channel);

	//send message to channel
	//static string read/write inside
	void send_Message(Connection *c, const char *channel, const char *msg);

	//populates message table with unread messages from the chat
	//returns if connection is closed
	//call this repeatedly in a loop
	void communicate(Message::Table *t, Connection *c, unsigned int timestamp);
}

namespace Twitch
{
	//call once to initialize network
	void startup()
	{
		Simple_TCP::init_Network();
	}

	void init(Connection *c, const char *username, const char *oauth)
	{
		c->username = (char*)username;
		c->oauth = (char*)oauth;
		c->socket = INVALID_SOCKET;
		c->active = false;
	}

	//close active socket
	void close(Connection *c)
	{
		Simple_TCP::close(c->socket);
	}

	//creates a socket and makes a TCP connection
	//shutsdown connection if it already exists and reconnects
	void connect(Connection *c)
	{
		if (c->socket != INVALID_SOCKET)
		{
			close(c);
		}
	
		const char *hostname = "irc.chat.twitch.tv";
		int port = 6667;
		
		hostent *host = Simple_TCP::get_Host(hostname);
		Sleep(200);
		
		c->socket = Simple_TCP::make_Connection(host, port);
		Sleep(200);

		int r;
		char tmp[1024];
		sprintf(tmp, "PASS oauth:%s\r\n",c->oauth);
		r = send(c->socket, tmp, strlen(tmp), 0);
		sprintf(tmp, "NICK %s\r\n", c->username);
		r = send(c->socket, tmp, strlen(tmp), 0);
	}

	//update tcp buffer, responds to pings immediately
	//returns number of bytes read from socket
	int update(Connection *c)
	{
		c->active = true;
		int numbytes = Simple_TCP::read(c->buffer, buffer_size, c->socket);
		
		if (numbytes == 0)
		{
			c->active = false;
			return 0;
		}
		else if (numbytes < 0) return 0;
		
		return numbytes;
	}

	//joins a channel
	//static string read/write inside
	void join_Channel(Connection *c, const char *channel)
	{
		static char join_command[4096];
		sprintf(join_command, "JOIN #%s\r\n", channel);
		int r = send(c->socket, join_command, strlen(join_command), 0);
	}

	//send message to channel
	//static string read/write inside
	void send_Message(Connection *c, const char *channel, const char *msg)
	{
		static char command[4096];
		sprintf(command, "PRIVMSG #%s :%s\r\n", channel, msg);
		int r = send(c->socket, command, strlen(command), 0);
	}

	//populates message table with unread messages from the chat
	//each call clears out the previous list of messages
	//returns false if connection is closed
	void communicate(Message::Table *t, Connection *c, unsigned int timestamp)
	{
		Message::clear(t);

		int r = update(c);
		if (c->active == false) return;
		if (r == 0) return;

		//check each line for ping, system or privmsg
		char *line = strtok(c->buffer, "\n");
		for (;line!=NULL;)
		{
			const char *ping_msg = "PING :tmi.twitch.tv";
			if (strstr(line, ping_msg) != NULL)
			{
				const char *pong_msg = "PONG :tmi.twitch.tv\r\n";
				int r = send(c->socket, pong_msg, strlen(pong_msg), 0);
			}
			else if (strstr(line, "PRIVMSG") != NULL)
			{
				static char channel[64];
				static char username[64];
				static char message[1024];
				sscanf(line, ":%[^!]%*[^#]#%[^ ]%*[^:]:%[^\n]", username, channel, message);
				Message::add(t, channel, username, message, timestamp);
			}
			else
			{
				//do nothing
			}

			line = strtok(NULL, "\n");
		}

		return;
	}
}
