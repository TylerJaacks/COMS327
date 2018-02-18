// I got help from this website https://www.ploggingdev.com/2016/11/n-queens-solver-in-python-3/

#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

int queue_size = 8;

int board[8][8] = {
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 },
	{ 0 },{ 0 },{ 0 },{ 0 }
};

int main(int argc, char* argv)
{
	get_solutions(0);

	return 0;
}

void get_solutions(int col)
{
	if (col >= queue_size)
	{
		return;
	}

	for (int i = 0; i < queue_size; i++)
	{
		if (cant_attack(i, col))
		{
			board[i][col] = 1;
			
			if (col == queue_size - 1) 
			{
				print_solutions(8);

				board[i][col] = 0;
				
				return;
			}
				
			get_solutions(col + 1);
			board[i][col] = 0;
		}
			
	}
}

bool cant_attack(int row, int col)
{
	for (int i = 0; i < col; i++)
	{
		if (board[row][i] == 1)
		{
			return false;
		}
	}

	int i = row;
	int j = col;

	while (i >= 0 && j >= 0)
	{
		if (board[i][j] == 1)
		{
			return false;
		}

		i -= 1;
		j -= 1;
	}

	int k = row;
	int l = col;

	while (k < queue_size && l >= 0)
	{
		if (board[k][l] == 1)
		{
			return false;
		}

		k += 1;
		l -= 1;
	}

	return true;
}

void print_solutions(int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (board[i][j] == 1)
			{
				printf("%c%d", j + 97, i + 1);
			}
		}
	}

	printf("\n");
}