#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <lua5.3/lua.hpp>

#include "api.h"

void init_lua();
void register_lua_functions();
int load_script(const char* filename);
int load_all_scripts();

#endif