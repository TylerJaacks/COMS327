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
// TODO Draw coridors.
// TODO Fix random hardness generation.

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
	char file_hardness[21][81];
	room *rooms;
} file_format;

char screen[21][80];
char hardness[21][81];

room *dungeon_rooms;

int number_of_rooms;
uint32_t queue_size;

int main(int argc, char *argv[])
{
	FILE *file;
	file_format *format;

	number_of_rooms = 5;

	dungeon_rooms = calloc(number_of_rooms, sizeof(room));

	if (argc == 1)
	{
		srand(time(NULL));

		init_array();

		create_dungeon();

		generate_hardness();

		draw_screen();

		save_dungeon(file, format);
	}

	else 
	{
		if (!strcmp(argv[1], "-l"))
		{
			load_dungeon(file, format);
		}

		else if (!strcmp(argv[1], "-s"))
		{
			srand(time(NULL));

			init_array();

			create_dungeon();

			generate_hardness();

			draw_screen();

			save_dungeon(file, format);
		}

		else
		{
			printf("Bad arguments!\n");
		}
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
		for (int j = 0; j < 81; j++)
		{
			screen[i][j] = ' ';
			hardness[i][j] = '0';
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

void draw_hardness(file_format *data)
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			printf("%i ", &data->file_hardness[i][j]);
		}

		printf("\n");
	}
}

void read_binary(FILE *file, file_format *data)
{
	int length = strlen(getenv("HOME")) + strlen("/.rlg327/1521618087.rlg327") + 1;
	char *p = malloc(length * sizeof(char));

	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/1521618087.rlg327");

	file = fopen(p, "r");

	fread(&data->file_type, sizeof(char), 12, file);
	fread(&data->file_version, sizeof(uint32_t), 1, file);
	fread(&data->file_size, sizeof(uint32_t), 1, file);

	data->file_size = be32toh(data->file_size);

	fread(data->file_hardness, sizeof(char), 21*80, file);

	number_of_rooms = (data->file_size - 1700) / sizeof(room);

	fread(dungeon_rooms, sizeof(room), number_of_rooms, file);

	fclose(file);
}

void load_dungeon(FILE *file, file_format *format)
{
	read_binary(file, format);

	init_array();

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

	file = fopen(p, "w");

	char *marker = "RLG327-S2018";
	uint32_t version = 0;
	
	fwrite(marker, sizeof(char), 12, file);
	fwrite(&version, sizeof(uint32_t), 1, file);

	queue_size = (20 * sizeof(byte_t)) + ((80 * sizeof(byte_t)) * (21 * sizeof(byte_t))) + (4 * number_of_rooms);

	fwrite(&queue_size, sizeof(uint32_t), 1, file);

	fwrite(&hardness, sizeof(hardness), 1, file);

	fwrite(&dungeon_rooms, sizeof(dungeon_rooms), 1, file);

	printf("%d \n", queue_size);
}

void save_dungeon(FILE *file, file_format *data)
{
	write_binary(file, data);
}

// Corridor Code courtesy of Jeff Yokup.
void place_corridors()
{
	//int temp, x1, x2, y1, y2;

	//for (temp = 0; temp < number_of_rooms - 1; temp++) {
	//	x1 = dungeon_rooms[temp].x_pos;
	//	x2 = dungeon_rooms[temp + 1].x_pos;
	//	y1 = dungeon_rooms[temp].y_pos;
	//	y2 = dungeon_rooms[temp + 1].y_pos;

	//	placeHCorridor(x1, x2, y1, y2);
	//	placeVCorridor(x1, x2, y1, y2);
	//}
}

void placeVCorridor(int x1, int x2, int y1, int y2) {
	//int temp;
	//if (y1 > y2) { //1 must go up
	//	for (temp = y1; temp >= y2; temp--) {
	//		if (screen[temp][x2] == '.') {

	//		}
	//		else {
	//			screen[temp][x2] = '#';
	//		}
	//	}
	//}
	//else if (y1 < y2) { //1 must go down
	//	for (temp = y1; temp <= y2; temp++) {
	//		if (screen[temp][x2] == '.') {

	//		}
	//		else {
	//			screen[temp][x2] = '#';
	//		}
	//	}
	//}
}

void placeHCorridor(int x1, int x2, int y1, int y2) {
	//int temp;
	//if (x1 > x2) { //1 must go left
	//	for (temp = x1; temp >= x2; temp--) {
	//		if (screen[y1][temp] == '.') {

	//		}
	//		else {
	//			screen[y1][temp] = '#';
	//		}
	//	}
	//}
	//else if (x1 < x2) { //1 must go right
	//	for (temp = x1; temp <= x2; temp++) {
	//		if (screen[y1][temp] == '.') {

	//		}
	//		else {
	//			screen[y1][temp] = '#';
	//		}
	//	}
	//}
}

void generate_hardness()
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (screen[i][j] == '.') 
			{
				hardness[i][j] = '0';
			}

			else if (screen[i][j] == '#')
			{
				hardness[i][j] = '0';
			}

			else 
			{
				int rand_hardness = rand() % 255;

				hardness[i][j] = rand_hardness;
			}
		}
	}
}