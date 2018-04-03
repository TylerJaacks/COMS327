#ifndef MONSTER_H
#define MONSTER_H

#include "dice.h"

class monster
{
private:
    std::string name;
    std::string description;
    char symbol;
    std::vector<uint32_t> &color;
    dice &speed;
    uint32_t abilities;
    dice &hitpoints;
    dice &damage;
    uint32_t rarity;

public:
    monster();
    ~monster();

    void spawn_monster();
    void spawn_monster(uint32_t x, uint32_t y);
};
#endif