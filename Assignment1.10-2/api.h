#include <stdint.h>
#include <vector>
#include <cstdlib>
#include <lua5.3/lua.hpp>
#include <string>

#include "character.h"
#include "descriptions.h"
#include "dice.h"
#include "dims.h"
#include "dungeon.h"
#include "event.h"
#include "heap.h"
#include "io.h"
#include "move.h"
#include "npc.h"
#include "object.h"
#include "path.h"
#include "pc.h"
#include "utils.h"

// int32_t compare_characters_by_next_turn(lua_State* state);

static int lua_can_see(lua_State* state);