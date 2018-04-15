#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "io.h"
#include "object.h"

uint32_t pc_is_alive(dungeon_t *d)
{
  return d->PC && d->PC->alive;
}

void place_pc(dungeon_t *d)
{
  d->PC->position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->PC->position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));

  pc_init_known_terrain(d->PC);
  pc_observe_terrain(d->PC, d);

  io_display(d);
}

void config_pc(dungeon_t *d)
{
  static dice pc_dice(0, 1, 4);

  d->PC = new pc;

  d->PC->symbol = '@';

  place_pc(d);

  d->PC->speed = PC_SPEED;
  d->PC->alive = 1;
  d->PC->sequence_number = 0;
  d->PC->kills[kill_direct] = d->PC->kills[kill_avenged] = 0;
  d->PC->color.push_back(COLOR_WHITE);
  d->PC->hp = 100;
  d->PC->damage = &pc_dice;
  d->PC->name = "Isabella Garcia-Shapiro";

  d->character_map[character_get_y(d->PC)][character_get_x(d->PC)] = d->PC;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, d->PC)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, d->PC, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->PC->position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->PC->position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->PC->position[dim_x] + dir[dim_x],
              d->PC->position[dim_y] + dir[dim_y]) &&
      ((charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

uint32_t pc_in_room(dungeon_t *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->PC->position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->PC->position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->PC->position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->PC->position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}

void pc_learn_terrain(pc *p, pair_t pos, terrain_type_t ter)
{
  p->known_terrain[pos[dim_y]][pos[dim_x]] = ter;
  p->visible[pos[dim_y]][pos[dim_x]] = 1;
}

void pc_reset_visibility(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->visible[y][x] = 0;
    }
  }
}

terrain_type_t pc_learned_terrain(pc *p, int16_t y, int16_t x)
{
  if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) {
    io_queue_message("Invalid value to %s: %d, %d", __FUNCTION__, y, x);
  }

  return p->known_terrain[y][x];
}

void pc_init_known_terrain(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->known_terrain[y][x] = ter_unknown;
      p->visible[y][x] = 0;
    }
  }
}

void pc_observe_terrain(pc *p, dungeon_t *d)
{
  pair_t where;
  int16_t y_min, y_max, x_min, x_max;

  y_min = p->position[dim_y] - PC_VISUAL_RANGE;
  if (y_min < 0) {
    y_min = 0;
  }
  y_max = p->position[dim_y] + PC_VISUAL_RANGE;
  if (y_max > DUNGEON_Y - 1) {
    y_max = DUNGEON_Y - 1;
  }
  x_min = p->position[dim_x] - PC_VISUAL_RANGE;
  if (x_min < 0) {
    x_min = 0;
  }
  x_max = p->position[dim_x] + PC_VISUAL_RANGE;
  if (x_max > DUNGEON_X - 1) {
    x_max = DUNGEON_X - 1;
  }

  for (where[dim_y] = y_min; where[dim_y] <= y_max; where[dim_y]++) {
    where[dim_x] = x_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_x] = x_max;
    can_see(d, p->position, where, 1, 1);
  }
  /* Take one off the x range because we alreay hit the corners above. */
  for (where[dim_x] = x_min - 1; where[dim_x] <= x_max - 1; where[dim_x]++) {
    where[dim_y] = y_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_y] = y_max;
    can_see(d, p->position, where, 1, 1);
  }       
}

int32_t is_illuminated(pc *p, int16_t y, int16_t x)
{
  return p->visible[y][x];
}

void pc_see_object(character *the_pc, object *o)
{
  if (o) {
    o->has_been_seen();
  }
}

bool is_same_object_type(dungeon *d, object_type_t type)
{
  for(std::vector<object*>::size_type i = 0; i != d->PC->carry.size(); i++) 
  {
    if (d->PC->carry[i]->type == type)
    {
      return false;
    }
  }

  return true;
}

/* TODO Broken */
void wear_item(dungeon *d, int slot)
{
  switch (slot) 
  {
    /* Slot 1 */
    case 49:
      if (is_same_object_type(d, d->PC->carry[0]->type) == true)
      {
        d->PC->carry[0] = d->PC->equipped[0];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 2 */
    case 50:
      if (is_same_object_type(d, d->PC->carry[1]->type) == true)
      {
        d->PC->carry[1] = d->PC->equipped[1];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 3 */
    case 51:
      if (is_same_object_type(d, d->PC->carry[2]->type) == true)
      {
        d->PC->carry[2] = d->PC->equipped[2];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 4 */
    case 52:
      if (is_same_object_type(d, d->PC->carry[3]->type) == true)
      {
        d->PC->carry[3] = d->PC->equipped[3];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 5 */
    case 53:
      if (is_same_object_type(d, d->PC->carry[4]->type) == true)
      {
        d->PC->carry[4] = d->PC->equipped[4];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 6 */
    case 54:
      if (is_same_object_type(d, d->PC->carry[5]->type) == true)
      {
        d->PC->carry[5] = d->PC->equipped[5];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 7 */
    case 55:
      if (is_same_object_type(d, d->PC->carry[6]->type) == true)
      {
        d->PC->carry[6] = d->PC->equipped[6];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 8 */
    case 56:
      if (is_same_object_type(d, d->PC->carry[7]->type) == true)
      {
        d->PC->carry[7] = d->PC->equipped[7];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 9 */
    case 57:
      if (is_same_object_type(d, d->PC->carry[8]->type) == true)
      {
        d->PC->carry[8] = d->PC->equipped[8];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    /* Slot 10 */
    case 58:
      if (is_same_object_type(d, d->PC->carry[9]->type) == true)
      {
        d->PC->carry[9] = d->PC->equipped[9];
        d->PC->carry.erase(d->PC->carry.begin());
      }

      break;
    default:
      mvprintw(0, 0, "Invalid Carry Slot.");
  }
}

void remove_item(dungeon *d, int slot)
{
  switch (slot) {
    /* Slot 1 */
    case 49:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin());
      break;
    /* Slot 2 */
    case 50:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 1);
      break;
    /* Slot 3 */
    case 51:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 2);
      break;
    /* Slot 4 */
    case 52:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 3);
      break;
    /* Slot 5 */
    case 53:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 4);
      break;
    /* Slot 6 */
    case 54:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 5);
      break;
    /* Slot 7 */
    case 55:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 6);
      break;
    /* Slot 8 */
    case 56:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 7);
      break;
    /* Slot 9 */
    case 57:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 8);
      break;
    /* Slot 10 */
    case 58:
      d->PC->equipped = d->PC->carry;
      d->PC->carry.erase(d->PC->carry.begin() + 9);
      break;
    default:
      mvprintw(0, 0, "Invalid Carry Slot.");
  }
}

void drop_item(dungeon *d, int slot)
{
    switch (slot) {
    /* Slot 1 */
    case 49:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[0];

      d->PC->carry.erase(d->PC->carry.begin());
      break;
    /* Slot 2 */
    case 50:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[1];

      d->PC->carry.erase(d->PC->carry.begin() + 1);
      break;
    /* Slot 3 */
    case 51:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[2];

      d->PC->carry.erase(d->PC->carry.begin() + 2);
      break;
    /* Slot 4 */
    case 52:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[3];

      d->PC->carry.erase(d->PC->carry.begin() + 3);
      break;
    /* Slot 5 */
    case 53:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[4];

      d->PC->carry.erase(d->PC->carry.begin() + 4);
      break;
    /* Slot 6 */
    case 54:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[5];

      d->PC->carry.erase(d->PC->carry.begin() + 5);
      break;
    /* Slot 7 */
    case 55:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[6];

      d->PC->carry.erase(d->PC->carry.begin() + 6);
      break;
    /* Slot 8 */
    case 56:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[7];

      d->PC->carry.erase(d->PC->carry.begin() + 7);
      break;
    /* Slot 9 */
    case 57:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[8];

      d->PC->carry.erase(d->PC->carry.begin() + 8);
      break;
    /* Slot 10 */
    case 58:
      d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC->carry[9];

      d->PC->carry.erase(d->PC->carry.begin() + 9);
      break;
    default:
      mvprintw(0, 0, "Invalid Carry Slot.");
  }
}

void expunge_item(dungeon *d, int slot)
{
  switch (slot) {
    /* Slot 1 */
    case 49:
      d->PC->carry[0] = NULL;
      break;
    /* Slot 2 */
    case 50:
      d->PC->carry[1] = NULL;
      break;
    /* Slot 3 */
    case 51:
      d->PC->carry[2] = NULL;
      break;
    /* Slot 4 */
    case 52:
      d->PC->carry[3] = NULL;
      break;
    /* Slot 5 */
    case 53:
      d->PC->carry[4] = NULL;
      break;
    /* Slot 6 */
    case 54:
      d->PC->carry[5] = NULL;
      break;
    /* Slot 7 */
    case 55:
      d->PC->carry[6] = NULL;
      break;
    /* Slot 8 */
    case 56:
      d->PC->carry[7] = NULL;
      break;
    /* Slot 9 */
    case 57:
      d->PC->carry[8] = NULL;
      break;
    /* Slot 10 */
    case 58:
      d->PC->carry[9] = NULL;
      break;
    default:
      mvprintw(0, 0, "Invalid Carry Slot.");
  }
}

void list_carry(dungeon *d)
{
  // TODO List Carry
}

void list_equipped(dungeon *d)
{
  // TODO List Equip
}

void inspect_item(dungeon *d, int slot)
{
    switch (slot) {
    /* Slot 1 */
    case 49:
      mvprintw(26, 1, "%s", d->PC->carry[0]->description.c_str());
      break;
    /* Slot 2 */
    case 50:
      mvprintw(26, 1, "%s", d->PC->carry[1]->description.c_str());
      break;
    /* Slot 3 */
    case 51:
      mvprintw(26, 1, "%s", d->PC->carry[2]->description.c_str());
      break;
    /* Slot 4 */
    case 52:
      mvprintw(26, 1, "%s", d->PC->carry[3]->description.c_str());
      break;
    /* Slot 5 */
    case 53:
      mvprintw(26, 1, "%s", d->PC->carry[4]->description.c_str());
      break;
    /* Slot 6 */
    case 54:
      mvprintw(26, 1, "%s", d->PC->carry[5]->description.c_str());
      break;
    /* Slot 7 */
    case 55:
      mvprintw(26, 1, "%s", d->PC->carry[6]->description.c_str());
      break;
    /* Slot 8 */
    case 56:
      mvprintw(26, 1, "%s", d->PC->carry[7]->description.c_str());
      break;
    /* Slot 9 */
    case 57:
      mvprintw(26, 1, "%s", d->PC->carry[8]->description.c_str());
      break;
    /* Slot 10 */
    case 58:
      mvprintw(26, 1, "%s", d->PC->carry[9]->description.c_str());
      break;
    default:
      mvprintw(0, 0, "Invalid Carry Slot.");
  }

  refresh();
} 

void target_monster(dungeon *d)
{
  pair_t dest;
  int c;
  fd_set readfs;
  struct timeval tv;

  io_display_no_fog(d);

  mvprintw(0, 0, "Choose a monster, 't' to target the monster.");

  dest[dim_y] = d->PC->position[dim_y];
  dest[dim_x] = d->PC->position[dim_x];

  mvaddch(dest[dim_y] + 1, dest[dim_x], '*');
  refresh();

  do {
    do{
      FD_ZERO(&readfs);
      FD_SET(STDIN_FILENO, &readfs);

      tv.tv_sec = 0;
      tv.tv_usec = 125000; /* An eigth of a second */

      io_redisplay_non_terrain(d, dest);
    } while (!select(STDIN_FILENO + 1, &readfs, NULL, NULL, &tv));
    switch (mappair(dest)) {
    case ter_wall:
    case ter_wall_immutable:
    case ter_unknown:
      mvaddch(dest[dim_y] + 1, dest[dim_x], ' ');
      break;
    case ter_floor:
    case ter_floor_room:
      mvaddch(dest[dim_y] + 1, dest[dim_x], '.');
      break;
    case ter_floor_hall:
      mvaddch(dest[dim_y] + 1, dest[dim_x], '#');
      break;
    case ter_debug:
      mvaddch(dest[dim_y] + 1, dest[dim_x], '*');
      break;
    case ter_stairs_up:
      mvaddch(dest[dim_y] + 1, dest[dim_x], '<');
      break;
    case ter_stairs_down:
      mvaddch(dest[dim_y] + 1, dest[dim_x], '>');
      break;
    default:
      mvaddch(dest[dim_y] + 1, dest[dim_x], '0');
    }
    switch ((c = getch())) {
    case KEY_HOME:
      if (dest[dim_y] != 1) {
        dest[dim_y]--;
      }
      if (dest[dim_x] != 1) {
        dest[dim_x]--;
      }
      break;
    case KEY_UP:
      if (dest[dim_y] != 1) {
        dest[dim_y]--;
      }
      break;
    case KEY_PPAGE:
      if (dest[dim_y] != 1) {
        dest[dim_y]--;
      }
      if (dest[dim_x] != DUNGEON_X - 2) {
        dest[dim_x]++;
      }
      break;
    case KEY_RIGHT:
      if (dest[dim_x] != DUNGEON_X - 2) {
        dest[dim_x]++;
      }
      break;
    case KEY_NPAGE:
      if (dest[dim_y] != DUNGEON_Y - 2) {
        dest[dim_y]++;
      }
      if (dest[dim_x] != DUNGEON_X - 2) {
        dest[dim_x]++;
      }
      break;
    case KEY_DOWN:
      if (dest[dim_y] != DUNGEON_Y - 2) {
        dest[dim_y]++;
      }
      break;
    case KEY_END:
      if (dest[dim_y] != DUNGEON_Y - 2) {
        dest[dim_y]++;
      }
      if (dest[dim_x] != 1) {
        dest[dim_x]--;
      }
      break;
    case KEY_LEFT:
      if (dest[dim_x] != 1) {
        dest[dim_x]--;
      }
      break;
    case 033:
      return;
      break;
    }
  } while (c != 't');

  if (c == 't') 
  {
    if (charpair(dest) && charpair(dest) != d->PC) {
      io_queue_message("Targeting %s with an HP of %d", charpair(dest)->name, charpair(dest)->hp);
    }
  }

  pc_observe_terrain(d->PC, d);
  dijkstra(d);
  dijkstra_tunnel(d);

  io_display(d);
}