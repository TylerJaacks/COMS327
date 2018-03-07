#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"
#include "event.h"

#include "input.h"

#include <ncurses.h>


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

void do_combat(dungeon_t *d, character_t *atk, character_t *def)
{
  if (def->alive)
  {
    def->alive = 0;
    if (def != &d->pc)
    {
      d->num_monsters--;
    }
    atk->kills[kill_direct]++;
    atk->kills[kill_avenged] += (def->kills[kill_direct] +
                                 def->kills[kill_avenged]);
  }
}

void move_character(dungeon_t *d, character_t *c, pair_t next)
{
  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x])))
  {
    do_combat(d, c, charpair(next));
  }
  else
  {
    /* No character in new position. */

    d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character[c->position[dim_y]][c->position[dim_x]] = c;
  }
}

void do_moves(dungeon_t *d)
{
  pair_t next;
  character_t *c;
  event_t *e;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
   * use to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                       */

  if (pc_is_alive(d))
  {
    /* The PC always goes first one a tie, so we don't use new_event().  *
     * We generate one manually so that we can set the PC sequence       *
     * number to zero.                                                   */
    e = malloc(sizeof(*e));
    e->type = event_character_turn;
    /* Hack: New dungeons are marked.  Unmark and ensure PC goes at d->time, *
     * otherwise, monsters get a turn before the PC.                         */
    if (d->is_new)
    {
      d->is_new = 0;
      e->time = d->time;
    }
    else
    {
      e->time = d->time + (1000 / d->pc.speed);
    }
    e->sequence = 0;
    e->c = &d->pc;
    heap_insert(&d->events, e);
  }

  while (pc_is_alive(d) &&
         (e = heap_remove_min(&d->events)) &&
         ((e->type != event_character_turn) || (e->c != &d->pc)))
  {
    d->time = e->time;
    if (e->type == event_character_turn)
    {
      c = e->c;
    }
    if (!c->alive)
    {
      if (d->character[c->position[dim_y]][c->position[dim_x]] == c)
      {
        d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
      }
      if (c != &d->pc)
      {
        event_delete(e);
      }
      continue;
    }

    npc_next_pos(d, c, next);
    move_character(d, c, next);

    heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
  }

  if (pc_is_alive(d) && e->c == &d->pc)
  {
    int did_make_turn = 0;

    while (did_make_turn == 0)
    {
      int ch = getch();

      if (ch == KEY_BACKSPACE)
      {
        endwin();
        exit(1);
      }
      else if (ch == KEY_UP)
      {
        c = e->c;
        d->time = e->time;

        e->c = NULL;
        event_delete(e);
        pc_next_pos(d, next);

        next[dim_x] = c->position[dim_x];
        next[dim_y] = c->position[dim_y] - 1;

        if (mappair(next) <= ter_floor)
        {
          mappair(next) = ter_floor_hall;
        }

        move_character(d, c, next);

        did_make_turn = 1;
      }
      else if (ch == KEY_DOWN)
      {
        c = e->c;
        d->time = e->time;

        e->c = NULL;
        event_delete(e);
        pc_next_pos(d, next);

        next[dim_x] = c->position[dim_x];
        next[dim_y] = c->position[dim_y] + 1;

        if (mappair(next) <= ter_floor)
        {
          mappair(next) = ter_floor_hall;
        }

        move_character(d, c, next);

        did_make_turn = 1;
      }
      else if (ch == KEY_LEFT)
      {
        c = e->c;
        d->time = e->time;

        e->c = NULL;
        event_delete(e);
        pc_next_pos(d, next);

        next[dim_x] = c->position[dim_x] - 1;
        next[dim_y] = c->position[dim_y];

        if (mappair(next) <= ter_floor)
        {
          mappair(next) = ter_floor_hall;
        }

        move_character(d, c, next);

        did_make_turn = 1;
      }
      else if (ch == KEY_RIGHT)
      {
        c = e->c;
        d->time = e->time;

        e->c = NULL;
        event_delete(e);
        pc_next_pos(d, next);

        next[dim_x] = c->position[dim_x] + 1;
        next[dim_y] = c->position[dim_y];

        if (mappair(next) <= ter_floor)
        {
          mappair(next) = ter_floor_hall;
        }

        move_character(d, c, next);

        did_make_turn = 1;
      } else if (ch == KEY_F(1)) {
        WINDOW *my_win;
	      int startx, starty, width, height;
	      int ch;

          initscr();
          cbreak();
          curs_set(0);

          keypad(my_win, TRUE);

          height = 21;
          width = 80;

          starty = 0;
          startx = 0;

          refresh();
          my_win = create_newwin(height, width, starty, startx);

          pair_t p;

          npc_t *monster_list = calloc(d->num_monsters, sizeof(npc_t));

          int count = 0;
          int i = 0;

          for (p[dim_y] = 0; p[dim_y] < DUNGEON_Y; p[dim_y]++) {
            for (p[dim_x] = 0; p[dim_x] < DUNGEON_X; p[dim_x]++) {
              if (charpair(p)) {
                if (charpair(p)->symbol == '0') {
                  wprintw(my_win, "Stupid Monster (0)\n");
                } else if (charpair(p)->symbol == '1') {
                  wprintw(my_win, "Crazy Monster (1)\n");
                } else if (charpair(p)->symbol == '2') {
                  wprintw(my_win, "Tunneling Monster (2)\n");
                } else if (charpair(p)->symbol == '3') {
                  wprintw(my_win, "Crazy Tunneling Monster (3)\n");
                } else if (charpair(p)->symbol == '4') {
                  wprintw(my_win, "Telepathic Monster (4)\n");
                } else if (charpair(p)->symbol == '5') {
                  wprintw(my_win, "Crazy Telepatic Monster (5)\n");
                } else if (charpair(p)->symbol == '6') {
                  wprintw(my_win, "Tunneling Telepatic Monster (6)\n");
                } else if (charpair(p)->symbol == '7') {
                  wprintw(my_win, "Crazy Tunneling Telepatic Monster (7)\n");
                } else if (charpair(p)->symbol == '8') {
                  wprintw(my_win, "Smart Monster (8)\n");
                } else if (charpair(p)->symbol == '9') {
                  wprintw(my_win, "Smart Crazy Monster (9)\n");
                } else if (charpair(p)->symbol == 'A') {
                  wprintw(my_win, "Smart Tunneling Monster (A)\n");
                } else if (charpair(p)->symbol == 'B') {
                  wprintw(my_win, "Smart Tunneling Crazy Monster (B)\n");
                } else if (charpair(p)->symbol == 'C') {
                  wprintw(my_win, "Smart Telepathic Monster (C)\n");
                } else if (charpair(p)->symbol == 'D') {
                  wprintw(my_win, "Smart Telepathic Crazy Monster (D)\n");
                } else if (charpair(p)->symbol == 'E') {
                  wprintw(my_win, "Smart Tunneling Telepathic Monster (E)\n");
                } else if (charpair(p)->symbol == 'F') {
                  wprintw(my_win, "Smart Tunneling Telepathic Crazy Monster (F)\n");
                }
              }
            } 
          }

          wrefresh(my_win);

          while((ch = getch()) != KEY_F(1))
          {	

          }

          clear();

          render_dungeon(d);

          refresh();
        } 

        dijkstra(d);
        dijkstra_tunnel(d);
      }
    }
}

void dir_nearest_wall(dungeon_t *d, character_t *c, pair_t dir)
{
    dir[dim_x] = dir[dim_y] = 0;

    if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2)
    {
        dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
    }

    if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2)
    {
        dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
    }
}

uint32_t against_wall(dungeon_t *d, character_t *c)
{
    return ((mapxy(c->position[dim_x] - 1,
                 c->position[dim_y]) == ter_wall_immutable) ||
            (mapxy(c->position[dim_x] + 1,
                 c->position[dim_y]) == ter_wall_immutable) ||
            (mapxy(c->position[dim_x],
                 c->position[dim_y] - 1) == ter_wall_immutable) ||
            (mapxy(c->position[dim_x],
                 c->position[dim_y] + 1) == ter_wall_immutable));
}

uint32_t in_corner(dungeon_t *d, character_t *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x],
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x],
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

uint32_t move_pc(dungeon_t *d, uint32_t dir)
{
  return 1;
}

npc_t *generate_monster_list(dungeon_t *d)
{
  pair_t p;

  npc_t *monster_list = calloc(d->num_monsters, sizeof(npc_t));

  int count = 0;
  int i = 0;

  for (p[dim_y] = 0; p[dim_y] < DUNGEON_Y; p[dim_y]++)
  {
    for (p[dim_x] = 0; p[dim_x] < DUNGEON_X; p[dim_x]++)
    {
      if (charpair(p))
      {
        monster_list[i];

        i++;
      }
    }
  }

  return monster_list;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	
  WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0); 

	wrefresh(local_win);

	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	wrefresh(local_win);
	delwin(local_win);
}