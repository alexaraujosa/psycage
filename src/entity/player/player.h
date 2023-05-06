/***************************************************************
 * PLAYER MODULE                                               *
 *                                                             *
 * This module handles the player behavior.                    *
 ***************************************************************/

#ifndef __RL_PLAYER_H
#define __RL_PLAYER_H

#include "../entity.h"

typedef struct player {
    Entity entity;
    int level;
    int kills;
    int xp;
} PLAYER, *Player;

// #define PLAYER_SIZE ENTITY_SIZE

/* FUNCTION SIGNATURES */
Player defaultPlayer();
void destroyPlayer(Player player);

#endif