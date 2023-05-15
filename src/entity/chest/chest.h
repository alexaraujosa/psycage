/***************************************************************
 * CHEST MODULE                                                *
 *                                                             *
 * This module handles the existing chest in the game.         *
 ***************************************************************/

#ifndef __RL_CHEST_H
#define __RL_CHEST_H

#include "../entity.h"

typedef struct chest {
    Entity entity;
} CHEST, *Chest;

Chest defaultChest();
void destroyChest(Chest chest);

#endif