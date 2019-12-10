/*
30 pts
Closed hash table implementation - stores float data
*/

/*
25 pts
*/
int compress(char** dest, int* freq, char** src, int nsrc)
{
	/*
	given an array of strings with duplicates in src,
	populate dest array with the unique strings present in src and save their frequencies
	return number of strings in dest
	*/
}

/*
15 pts
*/
void sort(char** dest, char** src, int n)
{
	/*
	populate dest with alphabetically sorted strings in src
	*/
}

/*
read item database with prices and scanner data (shopping cart) from files.
Store item prices in a hash table.
Calculate the list of unique items in the shopping cart along with their frequencies.
Using the hash table to look up prices, prepare a receipt.
*/
int main()
{
	/*
	read in prices.txt and save item prices in your hash table
	
	file format:
	apple 0.54
	orange 0.86
	...

	5 pts
	*/

	/*
	read in scanner.txt and save items in an array of strings
	
	file format:
	apple
	apple
	orange
	apple
	orange
	...
	
	5 pts
	*/

	/*
	sort the item strings
	5 pts
	*/

	/*
	compress the item strings
	5 pts
	*/

	/*
	prepare and print the receipt by looking up prices in the hash table using the compressed strings	
	show the subtotal and total with tax

	file format:
	apple x3 (0.54 ea) 1.62
	orange x2 (0.86 ea) 1.72
	subtotal 3.34
	tax 0.28
	total 3.62

	5 pts
	*/

	
	return 0;
}