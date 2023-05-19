#ifndef __RL_ENTITY_AI_H
#define __RL_ENTITY_AI_H

#include <math.h>
#include "common.h"
#include "debug.h"
#include "util/math.h"
#include "../entity.h"
#include "./pathfind.h"
#include "../../gameloop/gameloop_common.h"
#include "../../map/map_common.h"

typedef struct gamestate GAMESTATE, *Gamestate;
extern Gamestate g_gamestate;
extern FILE* dbgOut;

typedef struct mob {
    Entity entity;
    int moveCooldown;
    int lastMove;
    int hasAI;
} MOB, *Mob;

/* FUNCTION SIGNATURES */
Mob defaultMob();
void destroyMob(Mob mob);
int attemptMoveMob(Coords playerCoords, Mob mob, int** map, int width, int height);
int addMobToMap(Mob mob, int** map, int width, int height);
void destroyMob(Mob mob);
int _isObstacle(int cell);

#endif