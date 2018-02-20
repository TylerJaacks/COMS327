#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

writeRoomsToDisc(FILE *f, struct Dungeon * currentDungeon)
{
	int numOfRooms, i;
	numOfRooms = currentDungeon->numberOfRooms;
	for (i = 0; i < numOfRooms; i++)
	{
		int y, x, height, width;
		y = currentDungeon->dungeonRooms[i].yPos;
		x = currentDungeon->dungeonRooms[i].xPos;
		height = currentDungeon->dungeonRooms[i].yHeight;
		width = currentDungeon->dungeonRooms[i].xWidth;

		fwrite((const void*)& y, sizeof(int), 1, f);
		fwrite((const void*)& x, sizeof(int), 1, f);
		fwrite((const void*)& height, sizeof(int), 1, f);
		fwrite((const void*)& width, sizeof(int), 1, f);
	}
}

writeHardnessToDisc(FILE *f, struct Dungeon * currentDungeon)
{
	int row, col;
	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 80; col++)
		{
			int tempHard = currentDungeon->hardness[row][col];
			fwrite((const void*)& tempHard, sizeof(int), 1, f);
		}
	}

}

int getFileSize(struct Dungeon * currentDungeon)
{
	int size = 0; //size of file in bytes
	size += 1700; //adding bytes for hardness 
	size += ((currentDungeon->numberOfRooms) * 4); //adding room bytes
	return size;
}

void saveToDisc(struct Dungeon * currentDungeon)
{ //Save the dungeon to disc before terminating
	printf("Saving in progress\n");
	FILE *f;

	int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1;
	printf("Length:%i\npath: %s\n", length, getenv("HOME"));
	char *p = malloc(length * sizeof(char));
	printf("Allocated memory\n");
	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/dungeon");
	printf("Path: %s\n", p);
	//f = fopen("/home/student/assignment1_02/.rlg327/dungeon","wb");
	printf("about to open file\n");
	f = fopen(p, "wb");


	//printf("Opened file, will now save to disc\n");
	char *ptr = "RLG327-S2018";
	fwrite(ptr, 1, 12, f);
	int fileVersion = 0; int fileSize = 0;
	fwrite((const void*)& fileVersion, sizeof(int), 1, f); //write file version
	fileSize = getFileSize(currentDungeon);
	fwrite((const void*)& fileSize, sizeof(int), 1, f); //Write file size
	//printf("File size should be:%i \n", fileSize);
	writeHardnessToDisc(f, currentDungeon);
	writeRoomsToDisc(f, currentDungeon);
	fclose(f);
}

bool checkFileBeg(FILE *f, struct Dungeon * currentDungeon)
{ //returns 1 if good file
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
			//printf("%c",buffer[temp]);
		}
		free(buffer);
		//printf("\n");
		//printf("Count: %i \n", count);
		return 1;
	}
	else
	{
		free(buffer);
		return 0;
	}

}

void finalizeLoadCode(struct Dungeon * currentDungeon)
{
	//init rooms
	int temp, row, col, x, y, width, height;
	//printf("Finishing up\n");
	for (temp = 0; temp < currentDungeon->numberOfRooms; temp++)
	{ //go through each room
		x = currentDungeon->dungeonRooms[temp].xPos; y = currentDungeon->dungeonRooms[temp].yPos;
		width = currentDungeon->dungeonRooms[temp].xWidth; height = currentDungeon->dungeonRooms[temp].yHeight;

		for (row = y; row < y + height; row++)
		{
			for (col = x; col < x + width; col++)
			{
				currentDungeon->dungeonFloor[row][col] = 1;
			}
		}
		//printf("Loaded Room:%i \n", temp);
	}
	//printf("Finished\n");
}

void initDungeonFromDisc(FILE *f, int size, struct Dungeon * currentDungeon)
{ //ATTENTION ISSUE here?
//init hardness
	int arr[1680];
	fread(arr, sizeof(arr), 1, f);

	int row, col, counter, numOfRooms;
	counter = 0;
	for (row = 0; row < 21; row++)
	{
		for (col = 0; col < 81; col++)
		{
			//printf("Hardness:%i \n", arr[counter]);
			currentDungeon->hardness[row][col] = arr[counter];
			counter++;
		}
	}
	//initialized hardness, now get rooms

	numOfRooms = (size - 1700) / 4;
	//printf("Number of Rooms: %i\n", numOfRooms);
	int roomArr[numOfRooms * 4];
	fread(roomArr, sizeof(roomArr), 1, f);

	int temp;
	currentDungeon->numberOfRooms = numOfRooms;
	//printf("Num Of Rooms:%i\n", numOfRooms);

	for (temp = 0; temp < numOfRooms; temp++)
	{
		//printf("yPos:%i \n", roomArr[temp * 4]);
		currentDungeon->dungeonRooms[temp].yPos = roomArr[temp * 4];
		currentDungeon->dungeonRooms[temp].xPos = roomArr[(temp * 4) + 1];
		currentDungeon->dungeonRooms[temp].yHeight = roomArr[(temp * 4) + 2];
		currentDungeon->dungeonRooms[temp].xWidth = roomArr[(temp * 4) + 3];
	}
	//printf("Finished for loop\n");

}

void loadHelper(FILE *f, struct Dungeon * currentDungeon)
{
	//printf("Started Load helper method()\n");
	fseek(f, 0, SEEK_SET);//!!!!!! reset to start of file
	fseek(f, 16, SEEK_SET); //offset stream to file size
	int * fileSize;
	int count;
	fileSize = (int*)malloc(sizeof(int) * 1);
	count = fread(fileSize, sizeof(int), 1, f);
	if (count)
	{
		//printf("File size: %i\n", *fileSize);
		initDungeonFromDisc(f, *fileSize, currentDungeon);
		finalizeLoadCode(currentDungeon);
	}
	else
	{
		printf("ERROR file size");
	}
}

void loadFromDisc(struct Dungeon * currentDungeon)
{ //load current Dungeon from disc & display
//printf("loading from disc\n");
	FILE *f;


	int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1;
	char *p = malloc(length * sizeof(char));
	strcpy(p, getenv("HOME"));
	strcat(p, "/.rlg327/dungeon");

	printf("About to open file\n");
	f = fopen(p, "rb");
	printf("Found file");

	bool goodFile = checkFileBeg(f, currentDungeon);
	//bool goodFile = 1;
	if (goodFile)
	{
		printf("GOOD FILE\n");
		loadHelper(f, currentDungeon);
		fclose(f);
	}
	else
	{
		printf("BAD FILE");
		fclose(f);
		return;
	}
}

void processInput(int argc, char* argv[], struct Dungeon * cur)
{
	//currentDungeon = cur;
	printf("%i is nor\n", cur->numberOfRooms);
	if (argc == 1)
	{
		//printf("Only 1 argument, creating dungeon now");
		createDungeon();
	}
	else
	{
		bool save = 0; bool load = 0;
		//printf("Number of flags:%i\n", (argc-1));
		int i;
		for (i = 1; i < argc; i++)
		{
			//printf("Flag%i:%s \n",i,argv[i]);
			if (strcmp(argv[i], "--save") == 0)
			{
				//printf("Save found\n");
				save = 1;
			}

			if (strcmp(argv[i], "--load") == 0)
			{
				//printf("Load found\n");
				load = 1;
			}

			if (strcmp(argv[i], "--nummon") == 0)
			{
				// TODO select the number of monsters to load.
			}
		}

		if (load)
		{
			loadFromDisc(cur);
			generateCorridors(); //still gotta make corridors
		}

		else
		{
			createDungeon();
		}

		if (save)
		{
			saveToDisc(cur);
		}

	}
}
