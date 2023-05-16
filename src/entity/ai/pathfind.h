/***************************************************************
 * AI PATHFIND SUBMODULE                                       *
 *                                                             *
 * This submodule handles the pathfind for the ai of the game. *
 *                                                             *
 * FAIR WARNING: My wheelchaired grandma runs faster than this.*
 ***************************************************************/


#ifndef __RL_ENTITY_AI_PATHFIND_H
#define __RL_ENTITY_AI_PATHFIND_H

#include <stdio.h>
#include <math.h>
#include "common.h"
#include "util/math.h"
#include "../../map/map_common.h"

typedef struct as_node {
    int x;
    int y;
    float g;
    int closed;
    struct as_node* parent;
} AS_NODE, *As_Node;

As_Node make_node(int x, int y, As_Node parent);
As_Node pathfind(int g_height, int g_width, int** grid, Coords pa, Coords pb, int* obstacles, int obsLen);
Coords* pf_reconstruct_path(As_Node path, int count);
int pf_get_path_cell_count(As_Node path);

#endif