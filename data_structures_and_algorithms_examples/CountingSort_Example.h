#pragma once
#include <string.h>

void counting_Sort_V1(int *data, int n_data, int *freq, int freq_size)
{
	memset(freq, 0, sizeof(int)*freq_size);
	//count data
	for (int i = 0; i < n_data; i++)
	{
		freq[data[i]]++;
	}

	//re-populate
	int k = 0;
	for (int i = 0; i < freq_size; i++)
	{
		for (int j = 0; j < freq[i]; j++)
		{
			data[k++] = i;
		}
	}
}

void counting_Sort_V2(int *sorted_data, int *data, int n_data, int *freq, int freq_size)
{
	//this is the implementation that you would find online.
	//the example data array here is an integer, therefore the frequency summation step is redundant. 
	//however, in practice,
	//you would use an extra auxiliary data array,
	//or an array of structs for this implementation

	memset(freq, 0, sizeof(int)*freq_size);
	//count data
	for (int i = 0; i < n_data; i++)
	{
		freq[data[i]]++;
	}

	//calculate running sum
	for (int i = 1; i < freq_size; i++)
	{
		freq[i] += freq[i - 1];
	}

	//re-populate
	for (int i = 0; i < n_data; i++)
	{
		//NOTE:
		//for a slightly slower but stable version, use 'n_data-i-1' instead of 'i'
		//in this particular function, stability is not an issue, however,
		//if the data was an array of structs or a parallel auxiliary data array was used,
		//stability COULD matter
		//you must reason about the data and stability requirements before making a decision
		int index = --freq[data[i]];
		sorted_data[index] = data[i];
	}

}


void counting_Sort_V3(int *sorted_index, int *data, int n_data, int *freq, int freq_size)
{
	//this implementation is very flexible when used along with auxiliary data

	memset(freq, 0, sizeof(int)*freq_size);

	for (int i = 0; i < n_data; i++)
	{
		freq[data[i]]++;
	}

	for (int i = 1; i < freq_size; i++)
	{
		freq[i] += freq[i - 1];
	}

	//'n_data - i -1' makes the sort stable. use 'i' otherwise.
	for (int i = 0; i < n_data; i++)
	{
		int index = n_data - i - 1;
		sorted_index[--freq[data[index]]] = index;
	}
}