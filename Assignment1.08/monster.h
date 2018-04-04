#ifndef MONSTER_H
#define MONSTER_H

using namespace std;

#include <stdio.h>
#include <string>
#include <vector>

#include <unistd.h>

#include "dice.h"

class monster
{
public:
    string name;
    string description;
    char symbol;
    vector<uint32_t> color;
    dice speed;
    uint32_t abilities;
    dice hitpoints;
    dice damage;
    uint32_t rarity;

public:
    monster(string _name, string _description, char _symbol, vector<uint32_t> _color, dice _speed, uint32_t _abilities, dice _hitpoints, dice _damage, uint32_t _rarity);
};

#endif