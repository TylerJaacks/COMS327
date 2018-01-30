#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>

#define byte_t unsigned char

typedef struct {
	uint8_t x_pos;
	uint8_t y_pos;
	uint8_t x_size;
	uint8_t y_size;
} room;

typedef struct {
	char file_type[12]; // RLG327-S201 first 0-11 bytes.
	uint32_t file_version; // File version (0) the next 12-15 bytes.
	uint32_t file_size; // File size the next 16-19 bytes.
	char file_hardness[21][80]; // Matrix of hardness of each cell, the next 20–1699 bytes.
	room file_rooms[]; //  Array of rooms in the dungeon, the next 1700–end bytes.
} file_format;

int main(int argc, char *argv)
{
	FILE *file;
	file_format *format;

	char *hardness[21][80];

	int number_of_rooms = 5;
	int size = (20 * sizeof(byte_t)) + ((80 * sizeof(byte_t)) * (21 * sizeof(byte_t))) + (4 * number_of_rooms);

	load_dungeon(file, hardness);

	return 0;
}

void read_binary(FILE *file, file_format *data)
{
	int length = strlen(getenv("HOME")) + strlen("/.rlg327/hello.rlg327") + 1;
	char *p = malloc(length * sizeof(char));

	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/1521618087.rlg327");

	file = fopen(p, "rb");

	free(p);

	char file_type[13];

	fread(&data->file_type, sizeof(char), 12, file);
	fread(&data->file_version, 1, sizeof(uint32_t), file);
	// TODO Fix Size.
	fread(&data->file_size, sizeof(uint32_t), 1, file);
	// TODO Fix Hardness.
	fread(&data->file_hardness, sizeof(data->file_hardness), 1, file);
	// TODO Add Rooms and Replace 5 with the number of rooms.
	for (int i = 0; i < 5; i++)
	{

	}

	fclose(file);
}

void load_dungeon(FILE *file, file_format *format)
{
	read_binary(file, format);

	// TODO Set hardness from the struct.
	// TODO Place rooms from the struct.
	printf("File Type: %s\n", format->file_type);
	printf("File Version: %d\n", format->file_version);
	printf("File Size: %d\n", format->file_size);
}

void write_binary(FILE *file, file_format *data)
{
	int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon.rlg327") + 1;
	char *p = malloc(length * sizeof(char));

	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/dungeon.rlg327");

	file = fopen(p, "wb");

	free(p);

	// TODO Save Struct to Disk

	fclose(file);
}

void save_dungeon(FILE *file, file_format *data)
{
	write_binary(file, data);
}