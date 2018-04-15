#ifndef LUA_H
#define LUA_H

#include <lua5.3/lua.hpp>
#include <string>

#include "api.h"

#endif

class lua 
{
private:
    lua_State *state;

public:
    lua();

public:
    bool execute_script(const char* filename);
    void register_functions(lua_State* state);

private:
    void log_error();
};