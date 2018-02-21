#ifndef DUNGEON
#define DUNGEON
#include <stdbool.h>

struct node
{
	int row;
	int col;
	int distance;
	bool visited;
	struct node *next;
};

struct Room
{
	int xPos; // x cord for topLeft of room
	int yPos; // y cord for topleft of room
	int xWidth;
	int yHeight;
};

struct Dungeon
{
	int hardness[21][80]; //represents hardness of all squares in dungeon
	int dungeonFloor[21][80]; //represents dungeon features. (ie. 0 = rock, 1 = room, 2 = corridor, 3 = player, 4 = monster)
	struct Room dungeonRooms[280]; //280 can't get above this number for # of rooms
	int numberOfRooms;
	int normalDM[21][80];
	int diggingDM[21][80];

	struct node nodes[21][80];

} currentDungeon;

void create_dungeon();

#endif 