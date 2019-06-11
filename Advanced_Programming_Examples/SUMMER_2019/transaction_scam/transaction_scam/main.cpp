
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

void process(const char *customer_list_filename)
{
	static char *customer_filenames[1000] = { 0 };
	int number_of_customers = 0;

	FILE *f = fopen(customer_list_filename, "r");
	for (;;)
	{
		char tmp[256];
		int r = fscanf(f, "%s", tmp);
		if (r != 1) break;

		customer_filenames[number_of_customers] = (char*)malloc(sizeof(char) * 256);
		strcpy(customer_filenames[number_of_customers], tmp);
		printf("%s\n", customer_filenames[number_of_customers]);
		number_of_customers++;
	}
	fclose(f);

	printf("number of customers: %d\n", number_of_customers);

	static char *customer_name[1000] = { 0 };
	static char *customer_phone[1000] = { 0 };

	static float *customer_transactions[1000] = { 0 };
	static int customer_number_of_transactions[1000] = { 0 };

	for (int i = 0; i < number_of_customers; i++)
	{
		FILE *f = fopen(customer_filenames[i], "r");
		if (f == NULL)
		{
			printf("file not found\n");
			return;
		}

		customer_name[i] = (char*)malloc(sizeof(char) * 256);
		fscanf(f, "%s", customer_name[i]);

		customer_phone[i] = (char*)malloc(sizeof(char) * 256);
		fscanf(f, "%s", customer_phone[i]);

		customer_transactions[i] = (float*)malloc(sizeof(float) * 1000);
		customer_number_of_transactions[i] = 0;
		for (;;)
		{
			int r = fscanf(f, "%f", 
				&customer_transactions[i][customer_number_of_transactions[i]]);
			if (r != 1) break;
			customer_number_of_transactions[i]++;
		}

		fclose(f);

	}

	//everything has been read
	static float customer_balance[1000] = { 0 };
	for (int i = 0; i < number_of_customers; i++)
	{
		customer_balance[i] = 0;
		for (int j = 0; j < customer_number_of_transactions[i]; j++)
		{
			customer_balance[i] += customer_transactions[i][j];
		}
	}

	for (int i = 0; i < number_of_customers; i++)
	{
		printf("*****\n");
		printf("name: %s\n", customer_name[i]);
		printf("phone: %s\n", customer_phone[i]);
		printf("number of transactions: %d\n", customer_number_of_transactions[i]);
		printf("balance: %.2f\n", customer_balance[i]);
	}

	int highest_balance_customer_index = 0;
	float highest_balance = customer_balance[0];
	for (int i = 1; i < number_of_customers; i++)
	{
		if (customer_balance[i] > highest_balance)
		{
			highest_balance_customer_index = i;
			highest_balance = customer_balance[i];
		}
	}

	printf("***REPORT***\n");
	printf("the richest customer is %s with balance of %.2f, please call %s\n",
		customer_name[highest_balance_customer_index],
		customer_balance[highest_balance_customer_index],
		customer_phone[highest_balance_customer_index]
	);


	for (int i = 1; i < number_of_customers; i++)
	{
		free(customer_filenames[i]);
		free(customer_name[i]);
		free(customer_phone[i]);
		free(customer_transactions[i]);
	}

}

int main(int argc, char **argv)
{
	if (argc != 2) return 0;

	const char *customer_list_filename = argv[1];
	printf("customer list at %s\n", customer_list_filename);

	process(customer_list_filename);
	process(customer_list_filename);
	process(customer_list_filename);

	return 0;
}