/***************************************************************
 * CHEST MODULE                                                *
 *                                                             *
 * This module handles the existing chest in the game.         *
 *                                                             *
 * NOTE: This module is like a Mystery Box: If you're lucky it *
 * will be easy to use and debug. If you're unlucky then you're*
 * all kinds of fucked, cause the stack trace will be long     *
 * enough to reach Australia.                                  *
 ***************************************************************/

#ifndef __RL_CHEST_H
#define __RL_CHEST_H

#include <math.h>
#include "common.h"
#include "../entity.h"
#include "../../map/map_common.h"

typedef struct chest {
    Entity entity;
} CHEST, *Chest;

Chest defaultChest();
void destroyChest(Chest chest);
int addChestToMap(Chest chest, int** map, int width, int height);


#endif