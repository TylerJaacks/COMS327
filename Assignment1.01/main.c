#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

typedef struct
{
	int x_pos;
	int y_pos;
	int x_size;
	int y_size;
} room;

typedef struct
{
	room rooms[5];
} dungeon;

char screen[21][80];

int main(int argc, char *argv)
{
	srand(time(NULL));

	int rand_x_pos = rand() % 10 + 1;
	int rand_y_pos = rand() % 10 + 1;
	int rand_x_size = rand() % 10 + 1;
	int rand_y_size = rand() % 10 + 1;

	init_array();

	place_room(rand_x_pos, rand_y_pos, 5, 5);

	print_array();

	return 0;
}

void place_room(int x_pos, int y_pos, int x_size, int y_size)
{
	printf("x_pos: %d y_pos: %d x_size: %d y_size: %d \n", x_pos, y_pos, x_size, y_size);

	for (int i = x_pos; i < x_pos + x_size; i++)
	{
		for (int j = y_pos; j < y_pos + y_size; j++)
		{
			screen[i][j] = '.';
		}
	}
}

bool does_conflict()
{

}

void init_array()
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			screen[i][j] = ' ';
		}
	}
}

void print_array()
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			printf("%c", screen[i][j]);
		}

		printf("\n");
	}
}