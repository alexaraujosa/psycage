#ifndef __RL_GAMELOOP_COMMON_H
#define __RL_GAMELOOP_COMMON_H

#include "../entity/player/player.h"
#include "../entity/projectile/projectile.h"
#include "../entity/ai/ai.h"

#define MAX_MESSAGES ALTURA_LOGO-1
#define MAX_MESSAGES_LENGTH LARGURA_LOGO*(4/3)

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
    char messages[MAX_MESSAGES][MAX_MESSAGES_LENGTH];
} GAMESTATE, *Gamestate;

extern Gamestate g_gamestate;
extern int** map_footprint;

#endif
