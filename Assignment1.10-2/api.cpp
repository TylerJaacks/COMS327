#include "api.h"

dungeon_t *d;

/* Returns the current dungeon. */
void set_dungeon(dungeon_t *d_ptr)
{
    d = d_ptr;
}

/* Gets the current dungeon. */
int lua_get_dungeon(lua_State *l)
{
    printf("GetDungeon method called.\n");
    int argc = lua_gettop(l);

    if (argc == 0) {
        /* TODO Gets the current dungeon. */
    } else {
        printf("Argument Errors.\n");
    }

    return 1;
}

/* Deletes a character */
int lua_character_delete(lua_State *l)
{
    printf("DeleteCharacter method called.\n");
    int argc = lua_gettop(l);

    character *character;

    /* TODO Get the character from the parameters. */

    if (argc == 1) {
        printf("Deleting Character.\n");
        character_delete(character);   
    } else {
        printf("Argument Errors.\n");
    }

    return 0;
}

int lua_character_get_pos(lua_State *l)
{
    printf("GetCharacterPos method called.\n");
    int argc = lua_gettop(l);

    if (argc == 0) {
        /* TODO Gets the characters position. */
    } else {
        printf("Argument Errors.\n");
    }

    return 1;
}

