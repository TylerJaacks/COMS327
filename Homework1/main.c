#include <stdio.h>

/* https://stackoverflow.com/questions/1921539/using-boolean-values-in-c Booleans in C. */
typedef int bool;
#define true 1
#define false 0

#define a 9 
#define b 10
#define c 11
#define d 12
#define e 13
#define f 14
#define g 15
#define h 16

int queens_position[4][4] = {{0,0}};

int main(int argc, char* argv)
{
	calculate();

	return 0;
}

bool calculate() 
{
	for (int i = 0; i < 10; i++) 
	{
		if (queens_position[i][i] == 0)
		{

		}

		return true;
	}

	return false;
}