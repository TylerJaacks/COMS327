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

room dungeon_rooms[5];

dungeon game_dungeon;

int main(int argc, char *argv)
{
	srand(time(NULL));

	init_array();

	create_dungeon();

	draw_screen();

	return 0;
}

room place_room(int x_pos, int y_pos, int x_size, int y_size)
{
	room new_room = {x_pos, y_pos, x_size, y_size};

	for (int i = x_pos; i < x_pos + x_size; i++)
	{
		for (int j = y_pos; j < y_pos + y_size; j++)
		{
			screen[i][j] = '.';
		}
	}
}

void create_dungeon()
{
	int n = 0;

	while (n < 5)
	{
		int rand_x_pos = rand() % 19 + 1;
		int rand_y_pos = rand() % 78 + 1;
		int rand_x_size = rand() % 5 + 4;
		int rand_y_size = rand() % 5 + 4;

		if (can_place(rand_x_pos, rand_y_pos, rand_x_size, rand_y_size))
		{
			place_room(rand_x_pos, rand_y_pos, rand_x_size, rand_y_size);
			room new_room = { rand_x_pos, rand_y_pos, rand_x_size, rand_y_size };
			n++;
			dungeon_rooms[n] = new_room;
		}
	}
}

bool can_place(int x_pos, int y_pos, int x_size, int y_size)
{
	if (x_pos + x_size - 1 >= 20 || y_pos + y_size - 1 >= 80)
	{
		return false;
	}

	for (int i = x_pos; i < x_pos + x_size; i++)
	{
		for (int j = y_pos; j < y_pos + y_size; j++)
		{
			if (screen[i+1][j] == '.')
			{
				return false;
			}

			if (screen[i - 1][j] == '.')
			{
				return false;
			}

			if (screen[i][j + 1] == '.')
			{
				return false;
			}

			if (screen[i][j - 1] == '.')
			{
				return false;
			}

			if (screen[i][j] == '.')
			{
				return false;
			}
		}
	}

	return true;
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

// TODO Implement
void place_corridors()
{

}

void draw_screen()
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