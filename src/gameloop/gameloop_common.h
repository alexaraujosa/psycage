#ifndef __RL_GAMELOOP_COMMON_H
#define __RL_GAMELOOP_COMMON_H

#include "util/list.h"

#include "../entity/player/player.h"
#include "../entity/projectile/projectile.h"
#include "../entity/ai/ai.h"
#include "../entity/chest/chest.h"

#define MAX_MESSAGES ALTURA_LOGO-1
#define MAX_MESSAGES_LENGTH LARGURA_LOGO*(4/3)

typedef struct mob MOB, *Mob;

typedef struct gamestate {
    Player player;         // Player structure
    Projectile* projectiles;
    int input_initialized; // Block user input
    int clock;             // General tick clock
    int block_clock;       // Stop clock execution
    LinkedList clocks;
    Mob* mobs;
    Chest* chests;
    int mob_count;
    int projectile_count;
    int chest_count;
    int last_res;
    int paused;
    char messages[MAX_MESSAGES][MAX_MESSAGES_LENGTH];
} GAMESTATE, *Gamestate;

extern Gamestate g_gamestate;
extern int** map_footprint;
extern int TICK_DURATION_MS;

#endif
