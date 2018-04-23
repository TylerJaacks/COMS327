#ifndef LUA_H
#define LUA_H

#include <lua5.3/lua.hpp>

#include "character.h"
#include "dungeon.h"

void set_dungeon(dungeon_t *d_ptr);

/* Character Functions */
int lua_character_delete(lua_State *l);
int lua_character_get_pos(lua_State *l);



#endif