/*
FAST WINDOWS FWC
provided as is
levent albayrak 2018
*/

#ifndef __FAST_WINDOWS_CONSOLE_H__
#define __FAST_WINDOWS_CONSOLE_H__
#include <stdio.h>
#include <Windows.h>
#include <assert.h>

namespace FWC
{
	void init();

	int width();
	int height();

	int key_State(unsigned char key);
	int key_Last_State(unsigned char key);

	void color(unsigned char color);
	void put(int x, int y, char c);
	void put(int x, int y, char* str);
	void vertical_Line(int x, int y, int height, char c);
	void horizontal_Line(int x, int y, int width, char c);
	void rectangle(int x, int y, int width, int height, char c);

	void clear();
	void present();
}

namespace FWC
{
	namespace Internals
	{
		CONSOLE_SCREEN_BUFFER_INFO G_buffer_info;
		RECT G_console_rect;

		HANDLE G_STD_HANDLE = NULL;
		HWND G_window = NULL;
		int G_width = 0;
		int G_height = 0;
		unsigned char G_brush = 0;

		CHAR_INFO *G_char_buffer = NULL;
		HANDLE G_front_buffer = NULL;
		HANDLE G_back_buffer = NULL;

		unsigned char G_key_state[256];
		unsigned char G_prev_key_state[256];
	}

	int width() { return Internals::G_width; }
	int height() { return Internals::G_height; }

	void update_Input()
	{
		memcpy(Internals::G_prev_key_state, Internals::G_key_state, 256);
		GetKeyState(0);
		GetKeyboardState(Internals::G_key_state);
	}

	void init()
	{
		Internals::G_STD_HANDLE = GetStdHandle(STD_INPUT_HANDLE);
		Internals::G_window = GetConsoleWindow();

		SetConsoleMode(Internals::G_STD_HANDLE, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

		COORD resized;
		resized.X = 80;
		resized.Y = 25;
		Internals::G_width = resized.X;
		Internals::G_height = resized.Y;

		Internals::G_char_buffer = new CHAR_INFO[Internals::G_width*Internals::G_height]; assert(Internals::G_char_buffer);

		Internals::G_front_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL); assert(Internals::G_front_buffer);
		Internals::G_back_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL); assert(Internals::G_back_buffer);

		SetConsoleScreenBufferSize(Internals::G_front_buffer, resized);
		SetConsoleScreenBufferSize(Internals::G_back_buffer, resized);

		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(Internals::G_front_buffer, &info);
		SetConsoleCursorInfo(Internals::G_back_buffer, &info);

		memset(Internals::G_key_state, 0, 256);
		memset(Internals::G_prev_key_state, 0, 256);

		SetConsoleActiveScreenBuffer(Internals::G_front_buffer);

		GetConsoleScreenBufferInfo(Internals::G_front_buffer, &Internals::G_buffer_info);
		GetClientRect(Internals::G_window, &Internals::G_console_rect);
	}

	int key_State(unsigned char key)
	{
		return (Internals::G_key_state[key] & 0x80) >> 7;
	}
	int key_Last_State(unsigned char key)
	{
		return (Internals::G_prev_key_state[key] & 0x80) >> 7;
	}	
	void color(unsigned char color)
	{
		Internals::G_brush = color;
	}
	void put(int x, int y, char c)
	{
		int index = y*Internals::G_width + x;
		Internals::G_char_buffer[index].Char.AsciiChar = c;
		Internals::G_char_buffer[index].Attributes = Internals::G_brush;
	}
	void put(int x, int y, char* str)
	{
		int len = strlen(str);
		CHAR_INFO *text_row = &Internals::G_char_buffer[Internals::G_width*y + x];
		for (int i = 0; i < len; i++)
		{
			text_row[i].Char.AsciiChar = str[i];
			text_row[i].Attributes = Internals::G_brush;
		}
	}
	void vertical_Line(int x, int y, int height, char c)
	{
		int from = y;
		int to = y + height;
		for (int i = from; i < to; i++)
		{
			int index = i*Internals::G_width + x;
			Internals::G_char_buffer[index].Char.AsciiChar = c;
			Internals::G_char_buffer[index].Attributes = Internals::G_brush;
		}
	}
	void horizontal_Line(int x, int y, int width, char c)
	{
		int from = x;
		int to = x + width;

		int index = y*Internals::G_width;
		for (int i = from; i < to; i++)
		{
			Internals::G_char_buffer[index + i].Char.AsciiChar = c;
			Internals::G_char_buffer[index + i].Attributes = Internals::G_brush;
		}
	}
	void rectangle(int x, int y, int width, int height, char c)
	{
		int from_y = y;
		int to_y = y + height;
		for (int i = from_y; i < to_y; i++)
		{
			CHAR_INFO *text_row = &Internals::G_char_buffer[Internals::G_width*i];

			int from_x = x;
			int to_x = x + width;
			for (int j = from_x; j < to_x; j++)
			{
				text_row[j].Char.AsciiChar = c;
				text_row[j].Attributes = Internals::G_brush;
			}
		}
	}
	void clear()
	{
		memset(Internals::G_char_buffer, 0, sizeof(CHAR_INFO)*Internals::G_width*Internals::G_height);
	}
	void present()
	{
		COORD buffer_size;
		buffer_size.Y = Internals::G_height;
		buffer_size.X = Internals::G_width;
		COORD origin = { 0,0 };
		SMALL_RECT src_rect;
		src_rect.Top = 0;
		src_rect.Left = 0;
		src_rect.Bottom = Internals::G_height - 1;
		src_rect.Right = Internals::G_width - 1;
		WriteConsoleOutput(Internals::G_back_buffer, Internals::G_char_buffer, buffer_size, origin, &src_rect);
		HANDLE tmp = Internals::G_back_buffer;
		Internals::G_back_buffer = Internals::G_front_buffer;
		Internals::G_front_buffer = tmp;
		SetConsoleActiveScreenBuffer(Internals::G_front_buffer);
	}
}

#endif