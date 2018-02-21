#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ROW 21
#define COL 80

struct Room
{
	int xPos;
	int yPos;
	int xWidth;
	int yHeight;
};

struct Dungeon
{
	int hardness[21][80]; 
	//represents dungeon features. (ie. 0 = rock, 1 = room, 2 = corridor)
	int dungeonFloor[21][80]; 
	struct Room dungeonRooms[280];
	int numberOfRooms;
	int distance_map[21][80];
} currentDungeon;

int spawn_row;
int spawn_col;

char cells[21][80];

struct Room getRandomRoom()
{ 
	int width = (rand() % 10) + 3;
	int height = (rand() % 10) + 2;
	struct Room temp;
	temp.xWidth = width;
	temp.yHeight = height;

	return temp;
}

int minimum[1680];
int _index = 0;

void init_array()
{
	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			if (currentDungeon.dungeonFloor[row][col] == 0)
			{
				cells[row][col] = 'X';
			}

			else if (currentDungeon.dungeonFloor[row][col] == 1)
			{
				cells[row][col] = '1';
			}

			else if (currentDungeon.dungeonFloor[row][col] == 2)
			{
				cells[row][col] = '1';
			}

			else if (currentDungeon.dungeonFloor[row][col] == 3)
			{
				cells[row][col] = 'S';
			}
		}
	}
}

bool in_graph(int *graph[21][80], int x, int y)
{
	if (graph[x][y] == 0)
	{
		return false;
	}

	else
	{
		return true;
	}
}

bool is_valid(int x, int y)
{
	if ((x >= 0 && x <= 20) && (y >= 0 && y <= 79))
	{
		return true;
	}

	else
	{
		return false;
	}
}

void find_min(int *graph[21][80], int length, int x, int y)
{
	int new_graph[21][80];

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			new_graph[i][j] = 0;
		}
	}

	if (cells[x][y] == 'E')
	{
		minimum[_index++] = length;
	}

	else if (cells[x][y] == 'X' || (cells[x][y] == '1' && in_graph(graph, x, y)))
	{
		return;
	}

	else if ((cells[x][y] == '1' && !in_graph(graph, x, y)) || cells[x][y] == 'S')
	{
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 80; j++)
			{
				new_graph[i][j] = graph[i][j];
			}
		}

		new_graph[x][y] = 1;

		length++;

		if (is_valid(x, y - 1))
		{
			find_min(new_graph, length, x, y - 1);
		}

		if (is_valid(x - 1, y))
		{
			find_min(new_graph, length, x - 1, y);
		}

		if (is_valid(x, y + 1))
		{
			find_min(new_graph, length, x, y + 1);
		}

		if (is_valid(x + 1, y))
		{
			find_min(new_graph, length, x + 1, y);
		}
	}
}

int calculate_distances()
{
	bool flag = 0;
	int min = 9999;
	int new_graph[21][80];

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			new_graph[row][col] = 0;
		}
	}

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			new_graph[row][col] = 0;
		}
	}

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			if (cells[row][col] == 'S')
			{
				find_min(new_graph, 0, row, col);
				flag = true;
				break;
			}
		}

		if (flag == true)
		{
			break;
		}
	}

	for (int i = 0; i < _index; i++)
	{
		if (minimum[i] < min)
		{
			min = minimum[i];
		}
	}

	return min;
}

void placeRoom(struct Room temp, int currentRooms)
{ 
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
		}
	}

	currentDungeon.dungeonRooms[currentRooms] = temp;
}

int roomConflict(struct Room newRoom, struct Room presentRoom)
{
	//returns a 1 if there would be a room conlfict if placing down newRoom,
	// 1 is for newRoom(A), 2 is for presentRoom(B)
	int newX2, newY2, presentX2, presentY2;

	//ex cord are (5,5) width 5, so brx would be 5 + 5 = 10 - 1 = 9
	newX2 = newRoom.xPos + newRoom.xWidth - 1; 
	presentX2 = presentRoom.xPos + presentRoom.xWidth - 1;

	newY2 = newRoom.yPos + newRoom.yHeight - 1;
	presentY2 = presentRoom.yPos + presentRoom.yHeight - 1;

	if (newRoom.xPos > presentX2 || newX2 < presentRoom.xPos)
	{ 
		//if one room is to the left of another
		return 0;
	}

	if (newRoom.yPos > presentY2 || newY2 < presentRoom.yPos)
	{
		return 0;
	}
	return 1;
}

int isAdjacent(struct Room newRoom, struct Room presentRoom)
{ 
	//Returns 1 if adjacent to another room
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

/* returns a 1 if safe to put down room(i.e. no conflicts with other rooms/outerwall)
 * currentRoomCount signfies how many rooms are currently in the room array in the dungeon struct, always filled from index 0, onwards
 */
int canPutDownRoom(struct Room newRoom, int currentRoomCount)
{
	int i;
	if (newRoom.xPos + newRoom.xWidth - 1 >= 80)
	{ 
		//if out of bounds horizontally
		return 0;
	}

	if (newRoom.yPos + newRoom.yHeight - 1 >= 21)
	{ 
		//if out of bounds vertically
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
{ 
	//generate 5 rooms
	int minRooms = 0; int attempts = 0;

	while (minRooms < 5)
	{ 
		//generate the minimum 5 rooms
		struct Room tempRoom = getRandomRoom();

		while (1)
		{ 
			// assign random x,y value to tempRoom, see if it can be put down, if not, increase attempt counter and try over again
			// 1 <-> 78
			int x = (rand() % 78) + 1;
			// 1 <-> 19
			int y = (rand() % 19) + 1;

			tempRoom.xPos = x;
			tempRoom.yPos = y;

			if (canPutDownRoom(tempRoom, minRooms))
			{ 
				// if the room can be put down
				placeRoom(tempRoom, minRooms);
				currentDungeon.numberOfRooms++;
				attempts = 0;
				minRooms++;
				break;
			}

			else
			{ 
				// increase attempts counter, if over 1000, get new Room dimensions.
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
	{ 
		//1 must go up
		for (temp = y1; temp >= y2; temp--)
		{
			if (currentDungeon.dungeonFloor[temp][x2] == 1)
			{

			}

			else
			{
				currentDungeon.dungeonFloor[temp][x2] = 2;
			}
		}
	}
	else if (y1 < y2)
	{ 
		//1 must go down
		for (temp = y1; temp <= y2; temp++)
		{
			if (currentDungeon.dungeonFloor[temp][x2] == 1)
			{

			}

			else
			{
				currentDungeon.dungeonFloor[temp][x2] = 2;
			}
		}
	}
}

void placeHCorridor(int x1, int x2, int y1, int y2)
{
	int temp;

	if (x1 > x2)
	{ 
		//1 must go left
		for (temp = x1; temp >= x2; temp--)
		{
			if (currentDungeon.dungeonFloor[y1][temp] == 1)
			{

			}

			else
			{
				currentDungeon.dungeonFloor[y1][temp] = 2;
			}
		}
	}
	else if (x1 < x2)
	{ 
		//1 must go right
		for (temp = x1; temp <= x2; temp++)
		{
			if (currentDungeon.dungeonFloor[y1][temp] == 1)
			{

			}

			else
			{
				currentDungeon.dungeonFloor[y1][temp] = 2;
			}
		}
	}
}

void generateCorridors()
{
	int temp, x1, x2, y1, y2;

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

void initBarrier()
{ 
	int i;

	for (i = 0; i < 21; i++)
	{ 
		currentDungeon.hardness[i][0] = 255;
		currentDungeon.hardness[i][79] = 255;
	}

	for (i = 0; i < 80; i++)
	{ 
		currentDungeon.hardness[0][i] = 255;
		currentDungeon.hardness[20][i] = 255;
	}

}

void printDungeon()
{
	int row, col, counter;
	counter = 0;

	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 80; col++)
		{
			if (currentDungeon.dungeonFloor[row][col] == 0)
			{
				//rock
				printf(" ");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 1)
			{ 
				//room
				printf(".");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 2)
			{ 
				//corridor
				printf("#");
			}

			else if (currentDungeon.dungeonFloor[row][col] == 3)
			{
				printf("@");
			}

			else
			{
				printf("Wrong Value:%i", currentDungeon.dungeonFloor[row][col]);
				printf("$$$");
				counter++;
			}

		}
		printf("\n");
	}
}

writeRoomsToDisc(FILE *f)
{
	int numOfRooms, i;
	numOfRooms = currentDungeon.numberOfRooms;
	for (i = 0; i < numOfRooms; i++)
	{
		int y, x, height, width;
		y = currentDungeon.dungeonRooms[i].yPos;
		x = currentDungeon.dungeonRooms[i].xPos;
		height = currentDungeon.dungeonRooms[i].yHeight;
		width = currentDungeon.dungeonRooms[i].xWidth;

		fwrite((const void*)& y, sizeof(int), 1, f);
		fwrite((const void*)& x, sizeof(int), 1, f);
		fwrite((const void*)& height, sizeof(int), 1, f);
		fwrite((const void*)& width, sizeof(int), 1, f);
	}
}

writeHardnessToDisc(FILE *f)
{
	int row, col;
	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 80; col++)
		{
			int tempHard = currentDungeon.hardness[row][col];
			fwrite((const void*)& tempHard, sizeof(int), 1, f);
		}
	}
}

int getFileSize()
{
	int size = 0; 

	size += 1700; 
	size += (currentDungeon.numberOfRooms * 4); 

	return size;
}

void saveToDisc()
{ 
	printf("Saving in progress\n");
	FILE *f;

	int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1;
	printf("Length:%i\n   path: %s", length, getenv("HOME"));
	char *p = malloc(length * sizeof(char));
	printf("Allocated memory\n");
	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/dungeon");

	//f = fopen("/home/student/assignment1_02/.rlg327/dungeon","wb");
	printf("about to open file\n");
	f = fopen(p, "wb");

	char *ptr = "RLG327-S2018";
	fwrite(ptr, 1, 12, f);
	int fileVersion = 0; int fileSize = 0;

	fwrite((const void*)& fileVersion, sizeof(int), 1, f);
	fileSize = getFileSize();

	fwrite((const void*)& fileSize, sizeof(int), 1, f); 
	writeHardnessToDisc(f);
	writeRoomsToDisc(f);
	fclose(f);
}

bool checkFileBeg(FILE *f)
{ 
	char * buffer; int count;

	buffer = (char*)malloc(sizeof(char) * 12);

	if (buffer == NULL)
	{
		free(buffer);
		printf("MEMORY ERROROROROROROR");
		return 0;
	}

	count = fread(buffer, sizeof(char), 12, f);

	if (count)
	{
		int temp;

		for (temp = 0; temp < 12; temp++)
		{

		}

		free(buffer);

		return 1;
	}

	else
	{
		free(buffer);
		return 0;
	}
}

void finalizeLoadCode()
{
	int x; 
	int y; 
	int width;
	int height;

	for (int temp = 0; temp < currentDungeon.numberOfRooms; temp++)
	{ 
		x = currentDungeon.dungeonRooms[temp].xPos; y = currentDungeon.dungeonRooms[temp].yPos;
		width = currentDungeon.dungeonRooms[temp].xWidth; height = currentDungeon.dungeonRooms[temp].yHeight;

		for (int row = y; row < y + height; row++)
		{
			for (int col = x; col < x + width; col++)
			{
				currentDungeon.dungeonFloor[row][col] = 1;
			}
		}
	}
}

void initDungeonFromDisc(FILE *f, int size)
{ 
	//ATTENTION ISSUE here?
	int arr[1680];
	fread(arr, sizeof(arr), 1, f);

	int row, col, counter, numOfRooms;
	counter = 0;

	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 81; col++)
		{
			currentDungeon.hardness[row][col] = arr[counter];
			counter++;
		}
	}

	numOfRooms = (size - 1700) / 4;

	int roomArr[numOfRooms * 4];

	fread(roomArr, sizeof(roomArr), 1, f);

	int temp;

	currentDungeon.numberOfRooms = numOfRooms;

	for (temp = 0; temp < numOfRooms; temp++)
	{
		currentDungeon.dungeonRooms[temp].yPos = roomArr[temp * 4];
		currentDungeon.dungeonRooms[temp].xPos = roomArr[(temp * 4) + 1];
		currentDungeon.dungeonRooms[temp].yHeight = roomArr[(temp * 4) + 2];
		currentDungeon.dungeonRooms[temp].xWidth = roomArr[(temp * 4) + 3];
	}
}

void loadHelper(FILE *f)
{
	//!!!!!! reset to start of file
	fseek(f, 0, SEEK_SET);
	//offset stream to file size
	fseek(f, 16, SEEK_SET);
	int * fileSize;
	int count;
	fileSize = (int*)malloc(sizeof(int) * 1);
	count = fread(fileSize, sizeof(int), 1, f);
	if (count)
	{
		initDungeonFromDisc(f, *fileSize);
		finalizeLoadCode();
	}
	else
	{
		printf("ERROR file size");
	}
}

void loadFromDisc()
{ 
	FILE *f;

	int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1;
	char *p = malloc(length * sizeof(char));

	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/dungeon");

	f = fopen(p, "rb");

	bool goodFile = checkFileBeg(f);

	if (goodFile)
	{
		loadHelper(f);
		fclose(f);
	}

	else
	{
		printf("BAD FILE");
		fclose(f);
		return;
	}
}

void create_dungeon()
{
	srand(time(NULL)); 

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			currentDungeon.dungeonFloor[row][col] = 0;
		}
	}

	initBarrier();
	generateRooms();
	generateCorridors();
}

processInput(int argc, char* argv[])
{

	if (argc == 1)
	{
		create_dungeon();
	}

	else
	{
		bool save = 0;
		bool load = 0;

		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "--save") == 0)
			{
				save = 1;
			}

			if (strcmp(argv[i], "--load") == 0)
			{
				load = 1;
			}
		}

		if (load)
		{
			//still gotta make corridors
			loadFromDisc();
			generateCorridors();
		}

		else
		{
			create_dungeon();
		}

		if (save)
		{
			saveToDisc();
		}

	}

	init_array();

	spawn_character();

	printDungeon();

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 80; col++)
		{
			char temp = cells[row][col];

			cells[row][col] = "E";

			int q = calculate_distances();

			cells[row][col] = q;

			init_array();

			cells[row][col] = temp;
		}
	}

	for (int row = 0; row < 21; row++)
	{
		for (int col = 0; col < 21; col++)
		{
			printf("%c", cells[row][col]);
		}

		printf("\n");
	}
}

void spawn_character()
{
	bool isSpawned = false;

	while (!isSpawned)
	{
		int rand_col = (rand() % 78) + 1;
		int rand_row = (rand() % 19) + 1;

		if (currentDungeon.dungeonFloor[rand_row][rand_col] == 1)
		{
			currentDungeon.dungeonFloor[rand_row][rand_col] = 3;

			spawn_row = rand_row;
			spawn_col = rand_col;

			cells[spawn_row][spawn_col] = 'S';

			isSpawned = true;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc > 3)
	{
		fprintf(stdout, "%s", "TOO MANY INPUTS\n");

		return -1;
	}

	else
	{
		for (int row = 0; row < 21; row++)
		{
			for (int col = 0; col < 80; col++)
			{
				currentDungeon.dungeonFloor[row][col] = 0;
				currentDungeon.hardness[row][col] = 255;
			}
		}

		processInput(argc, argv);

		return 0;
	}
}