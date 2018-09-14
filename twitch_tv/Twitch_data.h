#pragma once

namespace Twitch
{
	const int buffer_size = 1024 * 16;

	struct Connection
	{
		char *username;
		char *oauth;
		char buffer[buffer_size];
		SOCKET socket;
		bool active;
	};

	namespace Message
	{
		struct Table
		{
			char **channel;
			char **username;
			char **message;
			unsigned int *timestamp;
			int n_count;
			int array_size;
		};
	}
}

