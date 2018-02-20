#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "priority_queue.h"

typedef struct monster
{
	char type;
	bool intelligent;
	bool telepathy;
	bool tunneling;
	bool erratic;
	int speed;
} monster_t;

typedef struct game
{
	int total_turns;
	int current_turn;
} game_t;

void simulate_turn(game_t *game, heap_t *heap)
{
	int n = 5;

	if (game->current_turn == 0)
	{
		// Spawn Monsters
		for (int i = 0; i < n; i++)
		{
			monster_t *monster1 = { 'Z', true, false, true, false, 100 };
			monster_t *monster2 = { 'p', true, false, true, false, 100 };

			spawn_monster(heap, monster1);
			spawn_monster(heap, monster2);
		}
	}

	game->current_turn++;
	game->total_turns++;
}

void spawn_monster(heap_t *heap, monster_t *monster)
{
	push(heap, monster->speed, monster);
}

int main(int argc, char *argv[])
{
	heap_t *heap = (heap_t *)calloc(1, sizeof(heap_t));
	game_t *game = (game_t *)calloc(1, sizeof(game_t));

	int n = 10;

	for (int i = 0; i < n; i++)
	{
		printf("Taking turn %d \n", game->current_turn + 1);

		simulate_turn(game, heap);
	}

	printf("%c", pop);

	return 1;
}