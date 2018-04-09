#ifndef LUA_H
#define LUA_H

#include <lua5.3/lua.hpp>
#include <string>

#endif

class lua 
{
private:
    lua_State *state;

public:
    lua();

public:
    bool execute_script(const char* filename);

private:
    void log_error();
};