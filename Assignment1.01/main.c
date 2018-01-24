#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
	int hardness[21][80]
} dungeon;

char screen[21][80];
int hardness[21][80];

room dungeon_rooms[5];
dungeon game_dungeon;

int main(int argc, char *argv)
{
	int q = time(NULL);
	int i = 1516752056;

	printf("%i", i);

	srand(q);

	init_array();

	create_dungeon();

	draw_screen();

	//game_dungeon{ dungeon_rooms, hardness };

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
		int rand_x_pos = rand() % 78 + 1;
		int rand_y_pos = rand() % 19 + 1;
		int rand_x_size = rand() % 5 + 4;
		int rand_y_size = rand() % 5 + 4;

		if (can_place(rand_x_pos, rand_y_pos, rand_x_size, rand_y_size))
		{
			place_room(rand_x_pos, rand_y_pos, rand_x_size, rand_y_size);

			room new_room = { rand_x_pos, rand_y_pos, rand_x_size, rand_y_size };

			dungeon_rooms[n] = new_room;
			
			n++;
		}
	}

	place_corridors();
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
	int n = 0;

	while (n < 1)
	{
		// Calculate centers of each room.
		int room1_center_x = ((dungeon_rooms[n].x_pos + dungeon_rooms[n].x_size + dungeon_rooms[n].x_pos) / 2);
		int room1_center_y = ((dungeon_rooms[n].y_pos + dungeon_rooms[n].y_size + dungeon_rooms[n].y_pos) / 2);

		int room2_center_x = ((dungeon_rooms[n + 1].x_pos + dungeon_rooms[n + 1].x_size + dungeon_rooms[n + 1].x_pos) / 2);
		int room2_center_y = ((dungeon_rooms[n + 1].y_pos + dungeon_rooms[n + 1].y_size + dungeon_rooms[n + 1].x_pos) / 2);

		// Calculate the distance between both rooms.
		int room_x_distance = abs(room1_center_x - room2_center_x);
		int room_y_distance = abs(room1_center_y - room2_center_y);

		int half_x_distance = room_x_distance / 2;
		int half_y_distance = room_y_distance / 2;

		// Check if room is to the left or right or same.
		if (room1_center_x > room2_center_x)
		{
			for (int i = room1_center_x; i >= room2_center_x; i--)
			{
				if (screen[room1_center_y][i] != '.')
				{
					screen[room1_center_y][i] = '#';
				}
			}
		}

		if (room1_center_x < room2_center_x)
		{
			for (int i = room1_center_x; i > room2_center_x; i--)
			{
				if (screen[room1_center_y][i] != '.')
				{
					screen[room1_center_y][i] = '#';
				}
			}
		}

		if (room1_center_y > room2_center_y)
		{
			printf("%i", room1_center_y);

			for (int i = room1_center_y; i >= room2_center_y; i--)
			{
				if (screen[i][room2_center_x] != '.')
				{
					screen[i][room2_center_x] = '#';
				}
			}
		}

		if (room1_center_y > room2_center_y)
		{
		}

		// Travel the remaining x or y distance. 
		if (room1_center_x > room2_center_x)
		{
			for (int i = half_x_distance; i >= room2_center_x; i--)
			{
				if (screen[room2_center_y][i] != '.')
				{
					screen[room2_center_y][i] = '#';
				}
			}
		}


		n++;
	}
}

void generate_hardness()
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (screen[i][j] == '.' || screen[i][j] == '#')
			{
				hardness[i][j] = 0;
			}

			else
			{
				hardness[i][j] = rand() % 255 + 1;
			}
		}
	}
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