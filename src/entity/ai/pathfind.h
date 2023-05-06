#ifndef __RL_ENTITY_AI_JS_H
#define __RL_ENTITY_AI_JS_H

#include <math.h>
#include "common.h"
#include "util/math.h"
#include "../../map/map_common.h"

#define D 1
#define D2 sqrt(2)

typedef struct as_node {
    int x;
    int y;
    float f;
    float g;
    float h;
    struct as_node* parent;
} AS_NODE, *As_Node;

As_Node make_node(int x, int y, As_Node parent);
As_Node pathfind(int g_height, int g_width, int** grid, Coords pa, Coords pb, int* obstacles, int obsLen);
Coords* pf_reconstruct_path(As_Node path, int count);
int pf_get_path_cell_count(As_Node path);

#endif