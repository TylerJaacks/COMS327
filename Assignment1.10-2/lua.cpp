#include "lua.h"

/* Creates a new Lua state. */
lua::lua()
{
    state = luaL_newstate();

    luaL_openlibs(state);
}

/* Logs the error for the top of the Lua stack. */
void lua::log_error()
{
    const char* message = lua_tostring(state, -1);

    printf(message);

    lua_pop(state, 1);
}

/* Loads a given Lua script. */
bool lua::execute_script(const char* filename)
{
    int result = luaL_loadfile(state, filename);

    if (result != LUA_OK) 
    {
        log_error();

        return false;
    }

    result = lua_pcall(state, 0, LUA_MULTRET, 0);

    if (result != LUA_OK) {
        log_error();

        return false;
    }

    return true;
}