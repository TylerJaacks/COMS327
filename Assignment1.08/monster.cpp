#include "monster.h"

#include <string>
#include <vector>

#include <stdio.h>
#include <unistd.h>

#include "dice.h"

using namespace std;

monster::monster(string _name, string _description, char _symbol, vector<uint32_t> _color, dice _speed, uint32_t _abilities, dice _hitpoints, dice _damage, uint32_t _rarity)
{
    name = _name;
    description = _description;
    symbol = _symbol;
    color = _color;
    speed = _speed;
    abilities = _abilities;
    hitpoints = _hitpoints;
    damage = _damage;
    rarity = _rarity;
}