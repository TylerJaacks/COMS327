#include <stdio.h>

int main(int argc, char* argv)
{
	int board[4][4] = {
	/*
		0,0 0,1 0,2 0,3
		1,0 1,1 1,2 1,3
		2,0 ...........
		...............
	*/
		{0}, {0}, {0}, {0},
		{0}, {0}, {0}, {0},
		{0}, {0}, {0}, {0},
		{0}, {0}, {0}, {0}
	};

	int queenCount = 0;

	board[0][0] = 1;
	queenCount += 1;

	if (can_attack(1, 1, board) == 1)
	{
		printf("Can attack!\n");
	}

	else
	{
		board[1][1] = 1;
	}

	//draw_board(board);

	return 0;
}

int can_attack(int row, int col, int board[4][4])
{
	int can_attack = 0;

	// Check Rows
	for (int i = 0; i < 4; i++) 
	{
		if (board[row][i] == 1)
		{
			return 1;
		}
	}

	// Check Columns
	for (int i = 0; i < 4; i++)
	{
		if (board[i][col] == 1)
		{
			return 1;
		}
	}

	return can_attack;
}

// https://stackoverflow.com/questions/35936358/c-printing-a-2d-char-array
void draw_board(int board[4][4])
{
	for (int k = 0; k < 4; ++k) {
		for (int j = 0; j < 4; ++j) {
			printf(" %d ", board[k][j]);
		}
		printf("\n");
	}

	printf("\n");
}