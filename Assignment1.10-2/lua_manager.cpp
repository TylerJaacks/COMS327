#include "lua_manager.h"

lua_State *L;

void init_lua()
{
    L = luaL_newstate();
}

void register_lua_functions()
{
    lua_pushcfunction(L, lua_character_delete);
    lua_setglobal(L, "DeleteCharacter");
}

int load_script(const char* filename)
{
    int status = luaL_loadfile(L, filename);

    if(status == LUA_OK)
    {
        return lua_pcall(L, 0, LUA_MULTRET, 0);
    }

    else
    {
        printf("Failed to load scripts.\n");

        return -1;
    }    
}

int load_all_scripts()
{
    // TODO Load all scripts in a the .rlg327 folder.
    printf("Loading Lua Scripts...\n");

    printf("Finished loading Lua Scripts.\n");

    return 1;
}