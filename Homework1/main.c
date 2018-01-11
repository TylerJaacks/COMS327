#include <stdio.h>

int board[4][4] = {
	{ 1 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 }
};

int maxQueens = 4;

int main(int argc, char* argv)
{
	draw_board();

	board[1][1] = 1;

	draw_board();

	if (can_attack(1, 1) == 1)
	{
		printf("Can attack!");
	}

	return 0;
}

int recursion(int queens)
{
	int queenCount = 0;

	if (queenCount == maxQueens)
	{
		printf("Finished");

		return 0;
	}

	else
	{

	}
}

int can_attack(int row, int col)
{
	int can_attack = 0;

	// Check rows
	for (int i = 0; i < 4; i++)
	{
		if (board[row][i] == 1)
		{
			return 1;
		}
	}

	// Check columns
	for (int i = 0; i < 4; i++)
	{
		if (board[i][col] == 1)
		{
			return 1;
		}
	}

	// Check diagonals
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 1)
			{
				if (abs(board[i][0] - col) == abs(board[0][j] - row)) 
				{
					return 1;
				}
			}
		}
	}

	return can_attack;
}

// https://stackoverflow.com/questions/35936358/c-printing-a-2d-char-array
void draw_board()
{
	for (int k = 0; k < 4; ++k) {
		for (int j = 0; j < 4; ++j) {
			printf(" %d ", board[k][j]);
		}
		printf("\n");
	}

	printf("\n");
}