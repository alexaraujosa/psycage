/***************************************************************
 * MAP COMMON MODULE                                           *
 *                                                             *
 * This module holds common data to be used by the map itself  *
 * and other modules throughout the codebase                   *
 ***************************************************************/

#ifndef __RL_MAP_COMMON_H
#define __RL_MAP_COMMON_H

#include "common.h"

typedef struct coords {
    int x;
    int y;
} COORDS, *Coords;

/* FUNCTION SIGNATURES */
Coords defaultCoords();
void destroyCoords(Coords coords);

#endif