#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

typedef struct game
{
	int total_turns;
	int current_turn;
} game_t;

void simulate_turn()
{
	
}

int main(int argc, char *argv[])
{
	heap_t *h = (heap_t *)calloc(1, sizeof(heap_t));
	game_t *game = (game_t *)calloc(1, sizeof(game_t));

	int n = 10;

	for (int i = 0; i < n; i++)
	{
		printf("Taking turn %d \n", game->current_turn + 1);

		game->current_turn++;
		game->total_turns++;
	}

	return 1;
}