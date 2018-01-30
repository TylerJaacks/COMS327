#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <math.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

#define byte_t unsigned char

// TODO Fix saving files.
// TODO Fix copying hardness.
// TODO Draw coridors.
// TODO Command line switches.

typedef struct {
	uint8_t x_pos;
	uint8_t y_pos;
	uint8_t x_size;
	uint8_t y_size;
} room;

typedef struct {
	char file_type[12];
	uint32_t file_version;
	uint32_t file_size;
	char file_hardness[21][80];
} file_format;

typedef struct
{
	room rooms[5];
	int hardness[21][80]
} dungeon;

char screen[21][80];
int hardness[21][80];

dungeon game_dungeon;
room *dungeon_rooms;

int number_of_rooms;
int size;

int main(int argc, char *argv)
{
	number_of_rooms = 5;

	dungeon_rooms = calloc(number_of_rooms, sizeof(room));

	// TODO Get arguments working.
	if (argc == 1)
	{
		srand(time(NULL));

		init_array();

		create_dungeon();

		draw_screen();
	}

	else
	{
		FILE *file;
		file_format *format;

		size = (20 * sizeof(byte_t)) + ((80 * sizeof(byte_t)) * (21 * sizeof(byte_t))) + (4 * number_of_rooms);

		load_dungeon(file, hardness);

		save_dungeon(file, format);
	}

	return 0;
}

room place_room(int x_pos, int y_pos, int x_size, int y_size)
{
	room new_room = { x_pos, y_pos, x_size, y_size };

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
			if (screen[i + 1][j] == '.')
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

void read_binary(FILE *file, file_format *data)
{
	int length = strlen(getenv("HOME")) + strlen("/.rlg327/hello.rlg327") + 1;
	char *p = malloc(length * sizeof(char));

	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/1521618087.rlg327");

	file = fopen(p, "r");

	fread(&data->file_type, sizeof(char), 12, file);
	fread(&data->file_version, sizeof(uint32_t), 1, file);
	fread(&data->file_size, sizeof(uint32_t), 1, file);

	data->file_size = be32toh(data->file_size);

	fread(&data->file_hardness, sizeof(char), 21*80, file);

	number_of_rooms = (data->file_size - 1699) / sizeof(room);

	fread(dungeon_rooms, sizeof(room), number_of_rooms, file);

	fclose(file);
}

void load_dungeon(FILE *file, file_format *format)
{
	read_binary(file, format);

	init_array();

	// TODO Fix copying hardness
	memcpy(hardness, &format->file_hardness, sizeof(format->file_hardness));

	for (int i = 0; i < number_of_rooms; i++) {
		place_room(dungeon_rooms[i].x_pos, dungeon_rooms[i].y_pos, dungeon_rooms[i].x_size, dungeon_rooms[i].y_size);
	}

	place_corridors();

	draw_screen();
}

void write_binary(FILE *file, file_format *data)
{
	int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon.rlg327") + 1;
	char *p = malloc(length * sizeof(char));

	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/dungeon.rlg327");

	file = fopen(p, "wa");

	char *marker = "RLG327-S2018";
	int version = 0;
	
	fwrite(marker, sizeof(char), 12, file);
	fwrite(version, sizeof(uint32_t), 1, file);
	fwrite(size, sizeof(uint32_t), 1, file);
	fwrite(hardness, sizeof(hardness), 1, file);
	fwrite(dungeon_rooms, sizeof(room), number_of_rooms, file);
}

void save_dungeon(FILE *file, file_format *data)
{
	write_binary(file, data);
}

// TODO Implement this.
void place_corridors()
{

}

// TODO Fix this.
void generate_hardness()
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (screen[i][j] == '#' || screen[i][j] == '.') {
				hardness[i][j] = '0';
			}
			else {
				int rand_hardness = rand() % 255;

				hardness[i][j] = rand_hardness;
			}
		}
	}
}