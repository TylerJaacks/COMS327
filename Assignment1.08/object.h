#ifndef OBJECT_H
#define OBJECT_H

using namespace std;

#include <stdio.h>
#include <string>
#include <vector>

#include <unistd.h>

#include "dice.h"

class object
{
public:
    typedef enum object_type {
        objtype_no_type,
        objtype_WEAPON,
        objtype_OFFHAND,
        objtype_RANGED,
        objtype_LIGHT,
        objtype_ARMOR,
        objtype_HELMET,
        objtype_CLOAK,
        objtype_GLOVES,
        objtype_BOOTS,
        objtype_AMULET,
        objtype_RING,
        objtype_SCROLL,
        objtype_BOOK,
        objtype_FLASK,
        objtype_GOLD,
        objtype_AMMUNITION,
        objtype_FOOD,
        objtype_WAND,
        objtype_CONTAINER
    } object_type_t;

    std::string name, description;
    object_type type;
    uint32_t color;
    dice hit;
    dice damage;
    dice dodge;
    dice defence;
    dice weight;
    dice speed;
    dice attribute;
    dice value;
    bool artifact;
    uint32_t rarity;

public:
    object(std::string _name, 
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
            uint32_t _rarity);
};

#endif