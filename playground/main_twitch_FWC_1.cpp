#pragma warning(disable:4996)

#include "../include/twitch_tv/Twitch_core.h"
#include "../include/twitch_tv/Twitch_Message_core.h"
#include "../include/Fast_Windows_Console/Fast_Windows_Console.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
	FWC::init();

	int sys_log_x = 0;
	int sys_log_y = 0;
	int tmp_offset_x = 0;
	int tmp_offset_y = 0;

	int msg_x = 0;
	int msg_y = 1;
	
	FWC::clear();

	FWC::color(0x0F);
	FWC::put(sys_log_x + tmp_offset_x, sys_log_y+ tmp_offset_y++, "initializing network"); FWC::present();
	
	//initialize network
	Twitch::startup();

	const char *username = "plogp";
	const char *token = "zi5igvfgn3914hg3hczbe497m8uzpp";

	FWC::put(sys_log_x + tmp_offset_x, sys_log_y + tmp_offset_y++, "connecting to twitch.tv and logging in"); FWC::present();

	Twitch::Connection connection;
	Twitch::init(&connection, username, token);
	Twitch::connect(&connection);

	FWC::put(sys_log_x + tmp_offset_x, sys_log_y + tmp_offset_y++, "connecting to a channel"); FWC::present();

	Twitch::join_Channel(&connection, "shroud");

	FWC::put(sys_log_x + tmp_offset_x, sys_log_y + tmp_offset_y++, "initializing message table"); FWC::present();
	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	static char tmp_str[4096];
	
	unsigned int last_print_time = clock();
	int last_msg_count = 0;
	float avg_msg_rate = 0.0;
	float avg_msg_length = 0.0;

	int *histogram = new int[256]; assert(histogram);
	memset(histogram, 0, sizeof(int) * 256);
	int n_total_histogram = 0;
	int histogram_x = 0;
	int histogram_y = 10;

	Sleep(1000);

	for(;;)
	{
		FWC::clear();

		unsigned int timestamp = clock();
		
		Twitch::communicate(&incoming, &connection, timestamp);
		if (connection.active == false)
		{
			exit(0);
		}

		if ((double)(timestamp - last_print_time)/CLOCKS_PER_SEC > 2.0)
		{
			avg_msg_length = 0.0;
			for (int i = last_msg_count; i < incoming.n_count; i++)
			{
				avg_msg_length += strlen(incoming.message[i]);
			}
			avg_msg_length /= (incoming.n_count - last_msg_count);

			avg_msg_rate = (double)(incoming.n_count - last_msg_count) * CLOCKS_PER_SEC / (timestamp - last_print_time);
			last_print_time = timestamp;
			last_msg_count = incoming.n_count;
		}

		FWC::color(0x0F);
		sprintf(tmp_str, "received %d messages", incoming.n_count);
		FWC::put(msg_x, msg_y, tmp_str);
		sprintf(tmp_str, "message rate per second %.4f", avg_msg_rate);
		FWC::put(msg_x, msg_y + 1, tmp_str);	
		sprintf(tmp_str, "avg message length %.4f", avg_msg_length);
		FWC::put(msg_x, msg_y + 2, tmp_str);

		FWC::present();
	}
	
	getchar();
}