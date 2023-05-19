/***************************************************************
 * PLAYER MODULE                                               *
 *                                                             *
 * This module handles the player behavior.                    *
 ***************************************************************/

#ifndef __RL_PLAYER_H
#define __RL_PLAYER_H

#include "../entity.h"
#include "../../data/items.h"
#include "../cheats/cheats.h"

typedef struct player {
    Entity entity;
    DataItemNode item;
    Cheats cheats;
    int level;
    int kills;
    int xp;
    int last_direction;
    int class;
    int radius;
} PLAYER, *Player;

enum Classes{
    Priest,
    Detective,
    Mercenary
};

// #define PLAYER_SIZE ENTITY_SIZE

/* FUNCTION SIGNATURES */
Player defaultPlayer();
void destroyPlayer(Player player);
char* getClassInterface(int classe);
void verifyPlayerRadius();
void player_spawn(Player player, int **map, int HEIGHT, int WIDTH);

#endif
