#include <stdio.h>
#include <stdlib.h>


int main()
{
	//STACK
	float data[100];//contiguous block of 100 floats inside the stack

	data[0] = 0.2;//the first float
	data[3] = 0.6;//the fourth float, 3*4=12 bytes offset

	float* p = data;//copy the address

	printf("%f\n", p[0]);
	printf("%f\n", p[3]);

	float x = *(p + 3);//same thing as p[3]
	printf("%f\n", x);

	//HEAP
	static float data2[100];//heap

	//BILL GATES
	//C way
	float* p1 = (float*)malloc(sizeof(float) * 100);//pass malloc number of bytes you need
	free(p1);//releases the memory back
	//C++ way
	float* p2 = new float[100];
	delete[] p2;

	//USING SIZE VARIABLES
	const int size = 500;//must be constant
	float data3[size];
	
	getchar();
	return 0;
}






