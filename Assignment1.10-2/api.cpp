#include "api.h"

static int lua_can_see(lua_State* state)
{
    printf("can_see has been called from Lua.\n");

    int args_count = lua_gettop(state);

    dungeon *d = NULL; // = lua_to*(L, 1);
    pair_t voyeur = {4}; // = lua_to*(L, 2);
    pair_t exhibitionist = {4}; // = lua_to*(L, 3);
    int is_pc = 0; // = lua_to*(L, 4);
    int learn = 0; // = lua_to*(L, 5);

    uint32_t result = can_see(d, voyeur, exhibitionist, is_pc, learn);

    lua_pushnumber(state, result);
}