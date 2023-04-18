/***************************************************************
 * MAP MODULE                                                  *
 *                                                             *
 * This module handles the creation and manipulation of maps.  *
 ***************************************************************/

#ifndef __RL_MAP_H
#define __RL_MAP_H

typedef struct coords {
    int x;
    int y;
} COORDS, *Coords;

// #define COORDS_SIZE sizeof(int)*2

/* FUNCTION SIGNATURES */
Coords defaultCoords();
void destroyCoords(Coords coords);

#endif