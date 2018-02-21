#ifndef HELPER
#define HELPER
typedef struct monster
{
	int pos_x;
	int pos_y;
	int intelligent;
	int telepathy;
	int tunneling;
	int erratic;
	int speed;
} monster_t;

typedef struct player
{
	int pos_x;
	int pos_y;
	int speed;
} player_t;

typedef struct game
{
	int total_turns;
	int current_turn;

	monster_t *monsters;
} game_t;
#endif