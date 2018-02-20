#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "dungeon.h"
#include "input.h"

#include "priority_queue.h"

//QUEUE CODE-------------------------

/* structure of a QUEUE node */
struct queue
{
	struct node *start;
};

struct queue newQueue;
struct queue * qPointer = &newQueue;

int getQueueLength()
{
	struct node * nodePointer = qPointer->start;
	int count = 1;
	if (nodePointer == NULL)
	{
		printf("Lenth is 0\n");
		return 0;
	}
	while (1)
	{
		printf("searching. nodepointerDistance: %i  address: %p \n", nodePointer->distance, &(*nodePointer));
		if (nodePointer->next != NULL)
		{
			nodePointer = nodePointer->next;
			count++;
		}
		else
		{
			return count;
		}

		if (count % 10 == 0)
		{
			sleep(2);
			printf("\n");
		}
	}
}

//push node into queue. Larger values are at the end.
void push_old(struct node * newNode)
{
	if (newNode == NULL)
	{
		printf("Stack overflow \n");
		exit(0);
		return;
	}
	else
	{
		if (qPointer->start == NULL)
		{ //if queue is empty
			qPointer->start = newNode;
			qPointer->start->next = NULL;
			newNode->next = NULL; //double checking
			//printf("First Push() row: %i col:%i Distance: %i  address: %p \n", qPointer->start->row, qPointer->start->col,qPointer->start->distance, &(*(qPointer->start)));
		}
		else
		{

			struct node * previousNode = qPointer->start;
			struct node * nextNode = NULL;
			if (qPointer->start->next != NULL)
			{
				nextNode = (qPointer->start->next);
			}
			else
			{
				if (previousNode->distance < newNode->distance)
				{ //insert newNode after start
					previousNode->next = newNode;
					newNode->next = NULL;
					return;
				}
				else
				{ //insert newNode at start
					struct node * save = &(*(qPointer->start));
					qPointer->start = newNode;
					newNode->next = save;
					save->next = NULL;
					return;
				}
			}
			int counter = 0;
			while (1)
			{
				counter++;
				if (nextNode == NULL)
				{ //if at end of list
					previousNode->next = newNode;
					newNode->next = NULL;

					if ((&(*(nextNode))) == (&(*(qPointer->start))))
					{
						printf("start point error\n");
						exit(0);
					}
					break;
				}
				else
				{
					if ((&(*(nextNode))) == (&(*(qPointer->start))))
					{
						printf("start point error\n");
						exit(0);
					}
					if (nextNode->distance > newNode->distance)
					{//insert inbetween prev and next
						previousNode->next = newNode; //prev points to newNode
						newNode->next = nextNode; //new Node points to next
						break;
					}
					else
					{
						struct node * temp = &(*nextNode); //temp points to the address of nextNode pointer
						previousNode = &(*temp); //prev points to next
						nextNode = nextNode->next; //next points to next's next
					}
				}

				if (counter % 500 == 0)
				{
					printf("counter: %i nextNodeDistance: %i newNodeDistance: %i QueueLength: %i \n", counter, nextNode->distance, newNode->distance, getQueueLength());
				}
			}
			// printf("Pushed newNode row: %i col: %i distance: %i address: %p \n\n", newNode->row, newNode->col, newNode->distance,  &(*newNode));
		}
	}
}

// Pops off start node which has the smallest distance value
struct node * pop_old()
{
	struct node * nodePointer = NULL;
	if (qPointer->start == NULL)
	{
		//printf("Start is null,pop_old() is wasting your time\n");
		nodePointer = NULL;
		return nodePointer;
	}
	else
	{
		nodePointer = &(*(qPointer->start));
		if (qPointer->start->next == NULL)
		{
			qPointer->start = NULL;  //emptying queue
		}
		else
		{
			qPointer->start = &(*(qPointer->start->next));  //shortening queue
		}
		return nodePointer;
	}
}

/* -------------------------------- END QUEUE CODE -------------------------------- */

typedef struct monster
{
	char type;
	int intelligent;
	int telepathy;
	int tunneling;
	int erratic;
	int speed;
} monster_t;

typedef struct game
{
	int total_turns;
	int current_turn;

	monster_t *monsters;
} game_t;

int number_of_monsters;

monster_t *monsters;

void initalize_monsters(game_t *game, heap_t *heap)
{
	number_of_monsters = 10;

	monsters = calloc(number_of_monsters, sizeof(monster_t));

	game->monsters = monsters;

	int n = 0;

	while (n < number_of_monsters)
	{
		int rand_x_pos = rand() % 19 + 1;
		int rand_y_pos = rand() % 78 + 1;

		char monster_type = 'M';

		int rand_intelligent = rand() & 1;
		int rand_telepathy = rand() & 1;
		int rand_tunneling = rand() & 1;
		int rand_erratic = rand() & 1;

		int rand_speed = rand() % 200 + 1;

		if (can_spawn_monster(game, heap, rand_x_pos, rand_y_pos, rand_tunneling) == 1)
		{
			monster_t monster = { monster_type, rand_intelligent, rand_telepathy, rand_tunneling, rand_erratic, rand_speed };
			monster_t *monster_ptr = { monster_type, rand_intelligent, rand_telepathy, rand_tunneling, rand_erratic, rand_speed };

			game->monsters[n] = monster;

			/* 0000 */
			if (rand_intelligent == 0 && rand_telepathy == 0 && rand_tunneling == 0 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 4;
			}

			/* 0001 */
			else if (rand_intelligent == 0 && rand_telepathy == 0 && rand_tunneling == 0 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 5;
			}

			/* 0010 */
			else if (rand_intelligent == 0 && rand_telepathy == 0 && rand_tunneling == 1 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 6;
			}

			/* 0011 */
			else if (rand_intelligent == 0 && rand_telepathy == 0 && rand_tunneling == 1 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 7;
			}

			/* 0100 */
			else if (rand_intelligent == 0 && rand_telepathy == 1 && rand_tunneling == 0 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 8;
			}

			/* 0101 */
			else if (rand_intelligent == 0 && rand_telepathy == 1 && rand_tunneling == 0 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 9;
			}

			/* 0110 */
			else if (rand_intelligent == 0 && rand_telepathy == 1 && rand_tunneling == 1 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 10;
			}

			/* 0111 */
			else if (rand_intelligent == 0 && rand_telepathy == 1 && rand_tunneling == 1 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 11;
			}

			/* 1000 */
			else if (rand_intelligent == 1 && rand_telepathy == 0 && rand_tunneling == 0 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 12;
			}

			/* 1001 */
			else if (rand_intelligent == 1 && rand_telepathy == 0 && rand_tunneling == 0 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 13;
			}

			/* 1010 */
			else if (rand_intelligent == 1 && rand_telepathy == 0 && rand_tunneling == 1 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 14;
			}

			/* 1011 */
			else if (rand_intelligent == 1 && rand_telepathy == 0 && rand_tunneling == 1 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 15;
			}

			/* 1100 */
			else if (rand_intelligent == 1 && rand_telepathy == 1 && rand_tunneling == 0 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 16;
			}

			/* 1101 */
			else if (rand_intelligent == 1 && rand_telepathy == 1 && rand_tunneling == 0 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 17;
			}

			/* 1110 */
			else if (rand_intelligent == 1 && rand_telepathy == 1 && rand_tunneling == 1 && rand_erratic == 0)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 18;
			}

			/* 1111 */
			else if (rand_intelligent == 1 && rand_telepathy == 1 && rand_tunneling == 1 && rand_erratic == 1)
			{
				currentDungeon.dungeonFloor[rand_x_pos][rand_y_pos] = 19;
			}

			// TODO Implement Heap

			n++;
		}
	}
}

int can_spawn_monster(game_t *game, heap_t *heap, int row, int col, int can_tunnel)
{
	if (can_tunnel == 1)
	{
		return true;
	}

	else
	{
		if (currentDungeon.dungeonFloor[row][col] == 1)
		{
			return true;
		}

		if (currentDungeon.dungeonFloor[row][col] == 2)
		{
			return true;
		}
	}

	return false;
}

struct Room getRandomRoom()
{ //get a Room with random width/height, x/y pos UNINITIALIZED
	int width = (rand() % 10) + 3;
	int height = (rand() % 10) + 2;
	struct Room temp;
	temp.xWidth = width;
	temp.yHeight = height;

	return temp;
}

void placeRoom(struct Room temp, int currentRooms)
{ //place room in dungeon
	int width, height, row, col;
	width = temp.xWidth;
	height = temp.yHeight;

	for (row = temp.yPos; row < temp.yPos + height; row++)
	{
		for (col = temp.xPos; col < temp.xPos + width; col++)
		{
			if (row >= 21 || col >= 80)
			{
				break;
			}
			currentDungeon.dungeonFloor[row][col] = 1;
			currentDungeon.hardness[row][col] = 0;
		}
	}
	currentDungeon.dungeonRooms[currentRooms] = temp;
}

int roomConflict(struct Room newRoom, struct Room presentRoom)
{//returns a 1 if there would be a room conlfict if placing down newRoom,
// 1 is for newRoom(A), 2 is for presentRoom(B)
	int newX2, newY2, presentX2, presentY2;
	newX2 = newRoom.xPos + newRoom.xWidth - 1; //ex cord are (5,5) width 5, so brx would be 5 + 5 = 10 - 1 = 9
	presentX2 = presentRoom.xPos + presentRoom.xWidth - 1;

	newY2 = newRoom.yPos + newRoom.yHeight - 1;
	presentY2 = presentRoom.yPos + presentRoom.yHeight - 1;

	if (newRoom.xPos > presentX2 || newX2 < presentRoom.xPos)
	{ //if one room is to the left of another
		return 0;
	}
	if (newRoom.yPos > presentY2 || newY2 < presentRoom.yPos)
	{
		return 0;
	}
	return 1;
}

int isAdjacent(struct Room newRoom, struct Room presentRoom)
{ //Returns 1 if adjacent to another room
	if ((newRoom.xPos + newRoom.xWidth) == presentRoom.xPos || (presentRoom.xPos + presentRoom.xWidth == newRoom.xPos))
	{
		return 1;
	}
	if ((newRoom.yPos + newRoom.yHeight) == presentRoom.yPos || (presentRoom.yPos + presentRoom.yHeight) == newRoom.yPos)
	{
		return 1;
	}

	return 0;
}

/*
//returns a 1 if safe to put down room(i.e. no conflicts with other rooms/outerwall)
currentRoomCount signfies how many rooms are currently in the room array in the dungeon struct, always filled from index 0, onwards
 */
int canPutDownRoom(struct Room newRoom, int currentRoomCount)
{
	int i;
	if (newRoom.xPos + newRoom.xWidth - 1 >= 79)
	{ //if out of bounds horizontally
		return 0;
	}
	if (newRoom.yPos + newRoom.yHeight - 1 >= 20)
	{ //if out of bounds vertically
		return 0;
	}
	for (i = 0; i < currentRoomCount; i++)
	{
		if (roomConflict(newRoom, currentDungeon.dungeonRooms[i]) || isAdjacent(newRoom, currentDungeon.dungeonRooms[i]))
		{
			return 0;
		}
	}
	return 1;
}

void generateRooms()
{ //generate 5 rooms
	int minRooms = 0; int attempts = 0;

	while (minRooms < 5)
	{ //generate the minimum 5 rooms
		struct Room tempRoom = getRandomRoom();

		while (1)
		{ //assign random x,y value to tempRoom, see if it can be put down, if not, increase attempt counter and try over again
			int x = (rand() % 78) + 1; //1 <-> 78
			int y = (rand() % 19) + 1; //1 <-> 19
			tempRoom.xPos = x;
			tempRoom.yPos = y;
			if (canPutDownRoom(tempRoom, minRooms))
			{ //if the room can be put down
				placeRoom(tempRoom, minRooms);
				currentDungeon.numberOfRooms++;
				attempts = 0;
				minRooms++;
				break;
			}
			else
			{ //increase attempts counter, if over 1000, get new Room dimensions.
				attempts++;
				if (attempts > 1000)
				{
					tempRoom = getRandomRoom();
					attempts = 0;
				}
			}

		}
	}
	printf("\n");

}

void placeVCorridor(int x1, int x2, int y1, int y2)
{
	int temp;
	if (y1 > y2)
	{ //1 must go up
		for (temp = y1; temp >= y2; temp--)
		{
			if (currentDungeon.dungeonFloor[temp][x2] == 1)
			{

			}
			else
			{
				currentDungeon.dungeonFloor[temp][x2] = 2;
				currentDungeon.hardness[temp][x2] = 0;
			}
		}
	}
	else if (y1 < y2)
	{ //1 must go down
		for (temp = y1; temp <= y2; temp++)
		{
			if (currentDungeon.dungeonFloor[temp][x2] == 1)
			{

			}
			else
			{
				currentDungeon.dungeonFloor[temp][x2] = 2;
				currentDungeon.hardness[temp][x2] = 0;
			}
		}
	}
}

void placeHCorridor(int x1, int x2, int y1, int y2)
{
	int temp;
	if (x1 > x2)
	{ //1 must go left
		for (temp = x1; temp >= x2; temp--)
		{
			if (currentDungeon.dungeonFloor[y1][temp] == 1)
			{

			}
			else
			{
				currentDungeon.dungeonFloor[y1][temp] = 2;
				currentDungeon.hardness[y1][temp] = 0;
			}
		}
	}
	else if (x1 < x2)
	{ //1 must go right
		for (temp = x1; temp <= x2; temp++)
		{
			if (currentDungeon.dungeonFloor[y1][temp] == 1)
			{

			}
			else
			{
				currentDungeon.dungeonFloor[y1][temp] = 2;
				currentDungeon.hardness[y1][temp] = 0;
			}
		}
	}
}

//Go from 1 coordinates, to 2 coordinates.
void generateCorridors()
{
	int temp, x1, x2, y1, y2;
	//printf("NOR:%i\n",currentDungeon.numberOfRooms );
	for (temp = 0; temp < currentDungeon.numberOfRooms - 1; temp++)
	{
		x1 = currentDungeon.dungeonRooms[temp].xPos;
		x2 = currentDungeon.dungeonRooms[temp + 1].xPos;
		y1 = currentDungeon.dungeonRooms[temp].yPos;
		y2 = currentDungeon.dungeonRooms[temp + 1].yPos;
		placeHCorridor(x1, x2, y1, y2);
		placeVCorridor(x1, x2, y1, y2);
	}
}

/* Initialize hardness of outer wall of dungeon */
void initBarrier()
{ 
	for (int i = 0; i < 21; i++)
	{ 
		/* Set left and right columns of dungeon to invincible stone */
		currentDungeon.hardness[i][0] = 255;
		currentDungeon.hardness[i][79] = 255;

		currentDungeon.nodes[i][0].visited = 1;
		currentDungeon.nodes[i][79].visited = 1;
	}

	for (int i = 0; i < 80; i++)
	{ 
		/* Set top and bottom rows of dungeon to invincible stone */
		currentDungeon.hardness[0][i] = 255;
		currentDungeon.hardness[20][i] = 255;

		currentDungeon.nodes[0][i].visited = 1;
		currentDungeon.nodes[20][i].visited = 1;
	}
}

void printDungeon()
{
	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			if (currentDungeon.dungeonFloor[row][col] == 0)
			{ 
				/* Rock */
				printf(" ");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 1)
			{ 
				/* Room */
				printf(".");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 2)
			{ 
				/* Corridor */
				printf("#");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 3)
			{
				/* Player */
				printf("@");
			}

			/* Monsters the hacky way 4 - 20 */
			else if (currentDungeon.dungeonFloor[row][col] == 4)
			{
				printf("0");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 5)
			{
				printf("1");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 6)
			{
				printf("2");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 7)
			{
				printf("3");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 8)
			{
				printf("4");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 9)
			{
				printf("5");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 10)
			{
				printf("6");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 11)
			{
				printf("7");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 12)
			{
				printf("8");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 13)
			{
				printf("9");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 14)
			{
				printf("a");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 15)
			{
				printf("b");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 16)
			{
				printf("c");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 17)
			{
				printf("d");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 18)
			{
				printf("e");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 19)
			{
				printf("f");
			}

			else
			{
				printf("Wrong Value:%i", currentDungeon.dungeonFloor[row][col]);
				printf("$$$");
			}
		}

		printf("\n");
	}

	printf("\n\n");
}

//-----------------------------------New Code!!!--------------------------

void displayNormalDistanceMap(int playerRow, int playerCol)
{
	int row, col;
	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 80; col++)
		{
			if (row == playerRow && col == playerCol)
			{
				printf("@");
			}
			else if (currentDungeon.hardness[row][col] == 255)
			{
				printf("#");
			}
			else if (currentDungeon.nodes[row][col].distance > 250)
			{
				printf(" ");
			}
			else
			{
				printf("%i", currentDungeon.nodes[row][col].distance % 10);
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

void displayDiggingDistanceMap(int playerRow, int playerCol)
{
	int row, col;
	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 80; col++)
		{
			if (row == playerRow && col == playerCol)
			{
				printf("@");
			}
			else if (currentDungeon.hardness[row][col] == 255)
			{
				printf("#");
			}
			else
			{
				//printf("%i ",currentDungeon.nodes[row][col].distance);
				printf("%i", currentDungeon.nodes[row][col].distance % 10);
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

bool safetyCheck(int row, int col)
{ //return 1 if valid/safe to visit.
	if (row >= 20 || col >= 79 || row <= 0 || col <= 0)
	{
		//printf("Rejected node with row: %i col: %i distance: %i", row, col, currentDungeon.nodes[row][col].distance);
		return 0;
	}
	if (currentDungeon.nodes[row][col].visited == 1)
	{
		//printf("Rejected node with row: %i col: %i distance: %i", row, col, currentDungeon.nodes[row][col].distance);
		return 0;
	}
	if (currentDungeon.hardness[row][col] == 255)
	{
		printf("Rejected node with row: %i col: %i distance: %i", row, col, currentDungeon.nodes[row][col].distance);
		return 0;
	}

	return 1;
}

int rocksUpdated = 0;
int loweredInfinity = 0;

void compareDistances(int row, int col, int currentDistance, char mapType)
{ //map is D for digging monsters, anything else for nondigging
	bool safe = safetyCheck(row, col);

	if (currentDistance == INT_MAX - 2000)
	{
		printf("ERROR ERROR ");
		exit(0);
	}

	if (safe)
	{
		int curPlusOne = currentDistance + 1;
		//printf("curPlusOne: %i \n", curPlusOne);
		if (mapType == 'D')
		{
			if (currentDungeon.dungeonFloor[row][col] == 0)
			{ //if new node is rock
				if (currentDungeon.nodes[row][col].distance == INT_MAX - 2000)
				{
					currentDungeon.nodes[row][col].distance = curPlusOne + 2;
					if (currentDungeon.nodes[row][col].distance == INT_MAX - 2000)
					{
						printf("ERROR");
						exit(0);
					}
					//printf("rock new distance: %i loweredInfinity: %i   ", currentDungeon.nodes[row][col].distance, loweredInfinity);
					loweredInfinity++;
					rocksUpdated++;
					struct node * p = &(currentDungeon.nodes[row][col]);
					push_old(p);
				}
				else
				{
					int oldDistance = currentDungeon.nodes[row][col].distance + (currentDungeon.hardness[row][col] / 85);
					int newDistance = curPlusOne + (currentDungeon.hardness[row][col] / 85);
					if (newDistance < oldDistance)
					{
						currentDungeon.nodes[row][col].distance = newDistance;
						if (currentDungeon.nodes[row][col].distance == INT_MAX - 2000)
						{
							printf("ERROR");
							exit(0);
						}
						// printf("rock new distance: %i rocksUpdated: %i   ", currentDungeon.nodes[row][col].distance, rocksUpdated);
						//printf("!!!Old Distance: %i   New distance: %i  for rock at row: %i col:%i!!!!!!!!\n",oldDistance, newDistance, row, col); 
						struct node * p = &(currentDungeon.nodes[row][col]);
						push_old(p);
						rocksUpdated++;
					}
				}
			}
			else
			{

				if (curPlusOne < currentDungeon.nodes[row][col].distance)
				{ //if a tunnel / room
					currentDungeon.nodes[row][col].distance = curPlusOne;
					struct node * p = &(currentDungeon.nodes[row][col]);
					push_old(p);
				}
			}
		}
		else
		{
			if (currentDungeon.dungeonFloor[row][col] != 0)
			{
				if (curPlusOne < currentDungeon.nodes[row][col].distance)
				{ //if a tunnel / room
					currentDungeon.nodes[row][col].distance = curPlusOne;
					struct node * p = &(currentDungeon.nodes[row][col]);
					push_old(p);

				}

			}
		}
	}
}

void updateNeighbors(int row, int col, int currentDistance, char mapType)
{ //map is D for digging monsters, anything else for nondigging
	compareDistances((row + 1), col, currentDistance, mapType); //down
	compareDistances(row, (col + 1), currentDistance, mapType); //right
	compareDistances((row - 1), col, currentDistance, mapType); //up
	compareDistances((row - 1), (col - 1), currentDistance, mapType); //up left
	compareDistances(row, (col - 1), currentDistance, mapType); //left
	compareDistances((row + 1), (col - 1), currentDistance, mapType); //down left
	compareDistances((row + 1), (col + 1), currentDistance, mapType); //down right
	compareDistances((row - 1), (col + 1), currentDistance, mapType); //up right
}

void initDistanceMap(int row, int col, char type)
{ //D == digging distance map
	struct node * currentNode = &currentDungeon.nodes[row][col];
	currentDungeon.nodes[row][col].distance = 0;
	currentDungeon.nodes[row][col].visited = 1;
	int currentRow = row; int currentCol = col;
	int nodeCounter = 0; //number of nodes moved through

	qPointer->start = NULL;

	while (1)
	{
		nodeCounter++;
		if (type == 'D')
		{
			updateNeighbors(currentRow, currentCol, currentDungeon.nodes[currentRow][currentCol].distance, 'D'); //update distances of neighbors
		}
		else
		{
			updateNeighbors(currentRow, currentCol, currentDungeon.nodes[currentRow][currentCol].distance, 'Z');//update distances of neighbors
		}

		currentDungeon.nodes[currentRow][currentCol].visited = 1;
		currentNode = pop_old();
		if (currentNode == NULL)
		{
			//printf("Finished\n");
			break;
		}
		else
		{
			if (currentNode->distance == INT_MAX - 2000)
			{
				printf("pop_old error\n");
				exit(0);
			}
			//printf("\ncurrentNode from pop_old() row: %i col:%i Distance: %i address: %p\n", currentNode->row, currentNode->col,currentNode->distance, &(*currentNode));
			currentRow = currentNode->row;
			currentCol = currentNode->col;
		}
	}
	//printf("Went through %i nodes\n", nodeCounter);
}

void placePlayer()
{
	int x, y;
	int startRoom = (rand() % currentDungeon.numberOfRooms);
	x = currentDungeon.dungeonRooms[startRoom].xPos;
	y = currentDungeon.dungeonRooms[startRoom].yPos;
	currentDungeon.dungeonFloor[y][x] = 3;
}

heap_t *heap;
game_t *game;

void process_turn()
{

}

void createDungeon()
{
	initBarrier();
	generateRooms();
	generateCorridors();
	placePlayer();
	initalize_monsters(heap, game);
}

void initDungeonValues()
{
	int row, col;
	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 80; col++)
		{
			currentDungeon.dungeonFloor[row][col] = 0;
			currentDungeon.hardness[row][col] = 250;
			currentDungeon.nodes[row][col].distance = INT_MAX - 2000; //to prevent overflow  
			currentDungeon.nodes[row][col].visited = 0;
			currentDungeon.nodes[row][col].row = row;
			currentDungeon.nodes[row][col].col = col;
		}
	}
}

int main(int argc, char* argv[])
{
	heap = (heap_t *)calloc(1, sizeof(heap_t));
	game = (game_t *)calloc(1, sizeof(game_t));

	srand(time(NULL)); //init random seed with current time
	currentDungeon.numberOfRooms = 0;
	struct Dungeon * point = &currentDungeon;
	//printf("NOR: %i\n", point->numberOfRooms);
	if (argc > 3)
	{
		printf("TOO MANY INPUTS"); //ATTENTION change to print to error buffer
		return -1;
	}
	else
	{
		initDungeonValues();
		createDungeon();
		//processInput(argc, argv, point); //take care of load/save/dungeon creation
		printDungeon();
		int playerRow, playerCol, row, col;
		for (row = 0; row < 21; row++)
		{
			for (col = 0; col < 80; col++)
			{
				if (currentDungeon.dungeonFloor[row][col] == 3)
				{
					playerRow = row; playerCol = col;
				}
			}
		}

		initDistanceMap(playerRow, playerCol, 'Z');
		//displayNormalDistanceMap(playerRow, playerCol);

		for (row = 0; row < 21; row++)
		{ //reset visited & distance values
			for (col = 0; col < 80; col++)
			{
				currentDungeon.nodes[row][col].distance = INT_MAX - 2000; //to prevent overflow
				currentDungeon.nodes[row][col].visited = 0;
			}
		}

		initBarrier();

		initDistanceMap(playerRow, playerCol, 'D'); //infinite loop here
		//displayDiggingDistanceMap(playerRow, playerCol);

		return 0;
	}
}