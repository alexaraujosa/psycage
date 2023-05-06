/***************************************************************
 * Items MODULE                                                *
 *                                                             *
 * This module handles the existing items in the game.         *
 ***************************************************************/

#ifndef __RL_ITEMS_H
#define __RL_ITEMS_H

#include "../entity.h"

typedef struct item {
    Entity entity;
    int normal;
    int rare;
    int legendery;
    int plusdamage;
} ITEM, *Item;

Item defaultItem();
void destroyItem(Item item);

#endif