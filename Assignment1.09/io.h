#ifndef IO_H
# define IO_H
#include "dims.h"

typedef struct dungeon dungeon_t;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon_t *d);
void io_handle_input(dungeon_t *d);
void io_queue_message(const char *format, ...);
void io_display_no_fog(dungeon_t *d);
void io_redisplay_non_terrain(dungeon_t *d, pair_t dest);
static bool is_vowel(const char c);

#endif
