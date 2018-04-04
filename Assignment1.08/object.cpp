#include "object.h"

#include <string>
#include <vector>

#include <stdio.h>
#include <unistd.h>

#include "dice.h"

#include "descriptions.h"

using namespace std;

object::object(std::string _name, 
                std::string _description, 
                uint32_t _type, 
                uint32_t _color, 
                dice _hit, 
                dice _damage, 
                dice _dodge, 
                dice _defence, 
                dice _weight, 
                dice _speed, 
                dice _attribute, 
                dice _value, 
                bool _artifact, 
                uint32_t _rarity)
{
    name = _name;
    description = _description, 
    type = _type;
    color = _color;
    hit = _hit;
    damage = _damage; 
    dodge = _dodge;
    defence = _defence;
    weight = _weight;
    speed = _speed;
    attribute = _attribute;
    value = _value;
    artifact = _artifact;
    rarity =_rarity;
}