#include <stdio.h>
#pragma warning(disable:4996)


int main()
{
	const int array_size = 5;
	int queue[array_size];
	int offset = 0;
	int n_data = 0;

	for (;;)
	{

		printf(">");
		char line[20];
		fgets(line, 20, stdin);
		
		char cmd = 0;
		int arg = 0;
		int r = sscanf(line, "%c%d", &cmd, &arg);
		if (r == 2 && cmd == 'e')
		{
			//enqueue
			if (n_data >= array_size)
			{
				printf("queue full!\n");
			}
			else
			{
				queue[(offset + n_data) % array_size] = arg;
				n_data++;
			}
		}
		else if (r == 1 && cmd == 'd')
		{
			//dequeue
			if (n_data <= 0)
			{
				printf("queue is empty!\n");
			}
			else
			{
				printf("served customer %d\n", queue[offset]);
				offset = (offset + 1) % array_size;
				n_data--;
			}
		}
		else
		{
			printf("unknown command %s", line);
		}

		if (n_data != 0)
		{
			printf("queue\n");
			for (int i = 0; i < n_data; i++)
			{
				printf("[%d] customer %d\n",i+1, queue[(offset + i) % array_size]);
			}
			printf("\n");
		}

	}

	return 0;
}