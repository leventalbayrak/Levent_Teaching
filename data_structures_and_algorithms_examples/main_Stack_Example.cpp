#include "Fast_Windows_Console.h"
#include "Stack_Example.h"

void draw_Stack(int x, int y, Stack::Stack *s);

int main()
{
	srand(1234);

	FWC::init();

	Stack::Stack my_stack;
	Stack::init(&my_stack, 8);

	int x = 30;
	int y = 5;

	for (;;)
	{
		FWC::clear();
		//collect keyboard events
		FWC::update_Input();
		
		//push when D key is tapped, stack will resize if necessary
		//will not grow more than size 18 to stay within screen bounds
		if (FWC::key_State('D') == 1 && FWC::key_Last_State('D') == 0)
		{
			if (my_stack.n_data < 18)
			{
				//push a random number between 0 and 999
				int val = rand() % 1000;
				Stack::push(&my_stack, val);
			}
		}

		//if the stack is not empty, pop when A key is tapped
		if (FWC::key_State('A') == 1 && FWC::key_Last_State('A') == 0)
		{
			if (my_stack.n_data > 0)
			{
				Stack::pop(&my_stack);
			}
		}

		//do not go out of screen bounds
		
		draw_Stack(x, y, &my_stack);
	
		//present drawing
		FWC::present();
	}

	return 0;
}

void draw_Stack(int x, int y, Stack::Stack *s)
{
	static char tmp[128];

	FWC::color(0xB0);
	for (int i = 0; i < s->size; i++)
	{
		sprintf(tmp, "%10d", s->data[i]);
		FWC::put(x, y + i, tmp);
	}
	FWC::color(0xE0);
	for (int i = 0; i < s->n_data; i++)
	{
		sprintf(tmp, "%10d", s->data[i]);
		FWC::put(x, y + i, tmp);
	}

	if (s->n_data == s->size)
	{
		sprintf(tmp, "n_data == size %4d ->", s->n_data);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y + s->n_data, tmp);

		FWC::color(0xC0);
		FWC::put(x, y + s->n_data, "!resize next push!");
	}
	else if (s->n_data == 0)
	{
		sprintf(tmp, "n_data %4d ->", s->n_data);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y, tmp);
		
		sprintf(tmp, "size %4d ->", s->size);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y + s->size, tmp);

		FWC::color(0xC0);
		FWC::put(x, y - 1, "!cannot pop!");
	}
	else
	{
		sprintf(tmp, "n_data %4d ->", s->n_data);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y + s->n_data, tmp);

		sprintf(tmp, "size %4d ->", s->size);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y + s->size, tmp);
	}

	if (s->n_data == 1)
	{
		sprintf(tmp, "first and last %4d ->", s->n_data - 1);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y, tmp);
	}
	else if (s->n_data > 1)
	{
		sprintf(tmp, "first %4d ->", 0);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y, tmp);

		sprintf(tmp, "last %4d ->", s->n_data-1);
		FWC::color(0x0F);
		FWC::put(x - strlen(tmp), y + s->n_data - 1, tmp);
	}
}
