#include <stdio.h>
#include <stdlib.h>

#include "DA_int.h"
#include "DA_template.h"
int main()
{
	DA_int list;
	init(&list);

	add(&list, 5);

	DA<int> my_list;
	add(&my_list, 5);

	DA<double> my_list_float;
	add<double>(&my_list_float, 5.0);

	return 0;
}