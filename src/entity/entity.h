/***************************************************************
 * ENTITY MODULE                                               *
 *                                                             *
 * This module handles the entity behavior, both for player    *
 *   controlled characters, mobs, dropped items and all other  *
 *   miscelaneous entities                                     *
 ***************************************************************/

#ifndef __RL_ENTITY_H
#define __RL_ENTITY_H

#include "common.h"
#include "util/math.h"
#include "../map/map.h"

typedef struct entity {
    Coords coords;
    int class;
    int maxHealth;
    int health;
    int armor;
    int basedamage;
    int damage;
    int dead;
} ENTITY, *Entity;

enum Classes{
    Priest,
    Detective,
    Mercenary
};

/* FUNCTION SIGNATURES */
Entity defaultEntity();
void destroyEntity(Entity entity);
void damageEntity(Entity entity, int hp);
void healEntity(Entity entity, int hp);
char* getClassInterface(int classe);

#endif