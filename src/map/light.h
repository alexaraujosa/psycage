/***************************************************************
 * LIGHT MODULE                                                *
 *                                                             *
 * This module handles the lightmap.                           *
 *                                                             *
 * NOTE: FUCK YOU JORGE!                                       *
 ***************************************************************/

#ifndef __RL_LIGHTS_H
#define __RL_LIGHTS_H

#include "common.h"
#include "../render/render.h"
#include <math.h>

extern int **visible;

void init_light_map(int HEIGHT, int WIDTH);
void calculate_visibility(int x, int y, int **map, int HEIGHT, int WIDTH);
void print_light(WINDOW* win, int HEIGHT, int WIDTH, int beginY, int beginX);

#endif