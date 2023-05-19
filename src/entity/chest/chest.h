/***************************************************************
 * CHEST MODULE                                                *
 *                                                             *
 * This module handles the existing chest in the game.         *
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