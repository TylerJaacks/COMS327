#include <stdio.h>

int board[4][4] = {
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 }
};

int maxQueens = 8;

int main(int argc, char* argv)
{
	recursion(8);

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
	for (int i = 0; i < 8; i++)
	{
		if (board[row][i] == 1)
		{
			return 1;
		}
	}

	// Check columns
	for (int i = 0; i < 8; i++)
	{
		if (board[i][col] == 1)
		{
			return 1;
		}
	}

	// Check diagonals
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 8; j++)
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

void spawn_queen(int row, int col)
{
	if (can_attack(row, col) == 1)
	{
		printf("Can attack!\n\n");
	}

	else
	{
		printf("Can't attack!\n\n");

		board[row][col] = 1;
	}
}

char convert_row(int row)
{
	switch (row)
	{
	case 0:
		return 'a';
	case 1:
		return 'b';
	case 2:
		return 'c';
	case 3:
		return 'd';
	case 4:
		return 'e';
	case 5:
		return 'f';
	case 6:
		return 'g';
	case 7:
		return 'h';
	}

	return 'q';
}

char* get_string()
{
	char* solution = "";

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; i++)
		{
			if (board[i][j] == 1)
			{
				solution += convert_row(i) + j;
			}
		}
	}
}

// https://stackoverflow.com/questions/35936358/c-printing-a-2d-char-array
void draw_board()
{
	for (int k = 0; k < 8; ++k) 
	{
		for (int j = 0; j < 8; ++j) 
		{
			printf(" %d ", board[k][j]);
		}

		printf("\n");
	}

	printf("\n");
}