#pragma once

#include "Simple_TCP.h"
#include "Twitch_data.h"

#include <time.h>
#include <iostream>
#include <fstream>
#include <assert.h>
using namespace std;

namespace Twitch
{
	namespace Message
	{
		void init(Table *t);

		void resize(Table *t);

		void clear(Table *t);

		void add(Table *t, const char *channel, const char *username, const char *msg, unsigned int timestamp);

		void append(Table *dest, const Table *src);

		void copy(Table *dest, const Table *src);

	}
}

namespace Twitch
{
	namespace Message
	{
		void init(Table *t)
		{
			t->array_size = 1024;
			t->n_count = 0;
			//TODO
			//aloc one block this is ugly
			t->channel = (char**)malloc(sizeof(char*)*t->array_size); assert(t->channel);
			t->username = (char**)malloc(sizeof(char*)*t->array_size); assert(t->username);
			t->message = (char**)malloc(sizeof(char*)*t->array_size); assert(t->message);
			t->timestamp = (unsigned int*)malloc(sizeof(unsigned int)*t->array_size); assert(t->timestamp);
			for (int i = 0; i < t->array_size; i++)
			{
				t->channel[i] = (char*)malloc(64); assert(t->channel[i]);
				t->username[i] = (char*)malloc(64); assert(t->username[i]);
				t->message[i] = (char*)malloc(1024); assert(t->message[i]);//IRC protocol limit is 512
				t->timestamp[i] = 0;
			}
		}

		void resize(Table *t)
		{
			int prev_array_size = t->array_size;
			t->array_size += t->array_size >> 1;
			//TODO
			//do one block this is ugly
			t->channel = (char**)realloc (t->channel,sizeof(char*)*t->array_size); assert(t->channel);
			t->username = (char**)realloc(t->username, sizeof(char*)*t->array_size); assert(t->username);
			t->message = (char**)realloc(t->message, sizeof(char*)*t->array_size); assert(t->message);
			t->timestamp = (unsigned int*)realloc(t->timestamp, sizeof(unsigned int)*t->array_size); assert(t->timestamp);
			for (int i = prev_array_size; i < t->array_size; i++)
			{
				t->channel[i] = (char*)malloc(64); assert(t->channel[i]);
				t->username[i] = (char*)malloc(64); assert(t->username[i]);
				t->message[i] = (char*)malloc(1024); assert(t->message[i]);//IRC protocol limit is 512
				t->timestamp[i] = 0;
			}
		}
		void clear(Table *t)
		{
			t->n_count = 0;
		}

		void add(Table *t, const char *channel, const char *username, const char *msg, unsigned int timestamp)
		{
			if (t->n_count >= t->array_size) resize(t);
			
			strcpy(t->channel[t->n_count], channel);
			strcpy(t->username[t->n_count], username);
			strcpy(t->message[t->n_count], msg);
			t->timestamp[t->n_count] = timestamp;
			++t->n_count;
		}
		
		void append(Table *dest, const Table *src)
		{
			for (int i = 0; i < src->n_count; i++)
			{
				add(dest, src->channel[i], src->username[i], src->message[i], src->timestamp[i]);
			}
		}

		void copy(Table *dest, const Table *src)
		{
			clear(dest);
			append(dest, src);
		}

	}
}
