#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"
# include "character.h"
# include "dungeon.h"
# include "object.h"

class pc : public character {
 public:
  std::vector<object*> equipped;
  std::vector<object*> carry;

 public:
  ~pc() {}
  terrain_type_t known_terrain[DUNGEON_Y][DUNGEON_X];
  uint8_t visible[DUNGEON_Y][DUNGEON_X];
};

void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint32_t pc_next_pos(dungeon *d, pair_t dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);
void pc_learn_terrain(pc *p, pair_t pos, terrain_type_t ter);
terrain_type_t pc_learned_terrain(pc *p, int16_t y, int16_t x);
void pc_init_known_terrain(pc *p);
void pc_observe_terrain(pc *p, dungeon *d);
int32_t is_illuminated(pc *p, int16_t y, int16_t x);
void pc_reset_visibility(pc *p);

void check_carry(dungeon *d);

void wear_item(dungeon *d, int slot);
void remove_item(dungeon *d, int slot);
void drop_item(dungeon *d, int slot);
void expunge_item(dungeon *d, int slot);
void list_carry(dungeon *d);
void list_equipped(dungeon *d);
void inspect_item(dungeon *d, int slot);
void target_monster(dungeon *d);

#endif