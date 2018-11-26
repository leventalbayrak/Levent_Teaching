#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

int read_Binary_File(char *buffer, int buffer_size, const char *filename)
{
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	int filesize = ftell(f);
	fseek(f, 0, SEEK_SET);

	int number_of_bytes_to_read = 0;
	if (filesize > buffer_size) number_of_bytes_to_read = buffer_size;
	else number_of_bytes_to_read = filesize;

	fread(buffer, 1, number_of_bytes_to_read, f);
	
	fclose(f);
	
	return number_of_bytes_to_read;
}

int main()
{
	int buffer_size = 1024 * 1024 * 16;
	char *buffer = (char*)malloc(sizeof(char)*buffer_size);

	int number_of_bytes = read_Binary_File(buffer, buffer_size, "client.dylib");

	printf("%d bytes read\n", number_of_bytes);

	FILE *f = fopen("string_dump.txt", "w+");

	//state 0 = search
	//state 1 = write
	int state = 0;

	char tmp_str[256];
	int position_in_tmp_str = 0;

	int position_in_buffer = 0;

	for (;;)
	{
		if (position_in_buffer >= number_of_bytes) break;

		int is_upper_case = (buffer[position_in_buffer] >= 'A' && buffer[position_in_buffer] <= 'Z');
		int is_lower_case = (buffer[position_in_buffer] >= 'a' && buffer[position_in_buffer] <= 'z');

		if (state == 0)
		{
			if (is_lower_case || is_upper_case)
			{
				state = 1;
				position_in_tmp_str = 0;
			}
			else
			{
				position_in_buffer++;
			}
		}
		else if (state == 1)
		{
			if (is_lower_case || is_upper_case || position_in_tmp_str >= 256)
			{
				tmp_str[position_in_tmp_str] = buffer[position_in_buffer];
				position_in_tmp_str++;
				position_in_buffer++;
			}
			else
			{
				state = 0;
				tmp_str[position_in_tmp_str] = 0;

				if (position_in_tmp_str > 5)
				{
					fprintf(f, "byte[%d] %s\n",position_in_buffer, tmp_str);
				}
			}
		}

	}

	fclose(f);

	return 0;
}