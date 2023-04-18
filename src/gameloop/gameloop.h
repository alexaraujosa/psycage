/***************************************************************
 * ENTITY MODULE                                               *
 *                                                             *
 * This module handles the entity behavior, both for player    *
 *   controlled characters, mobs, dropped items and all other  *
 *   miscelaneous entities                                     *
 ***************************************************************/

#ifndef __RL_GAMELOOP_H
#define __RL_GAMELOOP_H

#include "common.h"

// Import paths from main
extern char* BIN_PATH;
extern char ASSET_DIR[PATH_MAX];

/* FUNCTION SIGNATURES */
void init_gameloop();
void tick();

#endif