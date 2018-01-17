#include <stdio.h>

#define rock ' '
#define floor '.'
#define corridor '#'

int height = 21;
int width = 80;

int number_of_rooms = 5;

int main(int argc, char *argv)
{
	create_dungeon(height, width, number_of_rooms);

	return 0;
}

void create_dungeon(int width, int height, int n)
{

}