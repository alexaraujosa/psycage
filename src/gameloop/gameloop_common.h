#ifndef __RL_GAMELOOP_COMMON_H
#define __RL_GAMELOOP_COMMON_H

#include "../entity/player/player.h"
#include "../entity/projectile/projectile.h"
#include "../entity/ai/ai.h"

typedef struct mob MOB, *Mob;

typedef struct gamestate {
    Player player;         // Player structure
    Projectile projectile;
    int input_initialized; // Block user input
    int clock;             // General tick clock
    int block_clock;       // Stop clock execution
    Mob* mobs;
    int mob_count;
    int last_res;
    int paused;
} GAMESTATE, *Gamestate;

extern Gamestate g_gamestate;
extern int** map_footprint;

#endif
