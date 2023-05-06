#include "pathfind.h"
#include <stdio.h>

// typedef struct Node {
//     int x;
//     int y;
//     int f;
//     int g;
//     int h;
//     struct NODE* parent;
// } NODE, *Node;

As_Node make_node(int x, int y, As_Node parent) {
    As_Node node = (As_Node)malloc(sizeof(AS_NODE));
    node->x = x;
    node->y = y;
    node->f = 0;
    node->g = 0;
    node->h = 0;
    node->parent = parent;
    return node;
}

As_Node pathfind(int g_height, int g_width, int** grid, Coords pa, Coords pb, int obstacles[], int obsLen) {
    const int minX = 0, maxX = g_width - 1;
    const int minY = 0, maxY = g_height - 1;

    As_Node openSet[g_width * g_height];
    int openSetSize = 0;

    As_Node closedSet[g_width * g_height];
    int closedSetSize = 0;

    As_Node start = make_node(pa->x, pa->y, NULL);
    openSet[openSetSize++] = start;

    while(openSetSize > 0) {
        As_Node q = openSet[0];
        int qi = 0;

        for (int i = 1; i < openSetSize; i++) {
            if (q->f < openSet[i]->f) {
                q->f = openSet[i]->f;
                qi = i;
            }
        }

        if (q->x == pb->x && q->y == pb->y) return q;

        for (int i = qi; i < openSetSize - 1; i++) {
            openSet[i] = openSet[i+1];
        }
        openSetSize--;
        closedSet[closedSetSize++] = q;

        const int positions[4][2] = { {0, 1}, {-1, 0}, {1, 0}, {0, -1} };

        As_Node children[4];
        int childrenSize = 0;
        for (int i = 0; i < 4; i++) {
            volatile int position[2] = {q->x + positions[i][0], q->y + positions[i][1] };

            if (position[0] > maxX || position[0] < minX || position[1] > maxY || position[1] < minY) continue;
            // if (grid[position[1]][position[0]] != clearCell) continue;
            int blocked = FALSE;
            for (int j = 0; j < obsLen; j++) {
                int test = grid[position[1]][position[0]];
                if (test == obstacles[j]) {
                    blocked = TRUE;
                    break;
                }
            }
            if (blocked) continue;

            As_Node child = make_node(position[0], position[1], q);
            children[childrenSize++] = child;
        }

        for (int i = 0; i < childrenSize; i++) {
            As_Node child = children[i];

            if (child->x == pb->x && child->y == pb->y) return child;

            int childInClosedSet = FALSE;
            for (int j = 0; j < closedSetSize; j++) {
                if (child->x == closedSet[j]->x && child->y == closedSet[j]->y) {
                    childInClosedSet = TRUE;
                    break;
                }
            }
            if (childInClosedSet) continue;

            float gScore = child->parent->g + 1;
            int bestGScore = FALSE;

            int childInOpenSet = FALSE;
            for (int j = 0; j < openSetSize; j++) {
                if (child->x == openSet[j]->x && child->y == openSet[j]->y) {
                    childInOpenSet = TRUE;
                    break;
                }
            }

            if (!childInOpenSet) {
                bestGScore = TRUE;

                int d1 = abs(pb->x - child->x);
                int d2 = abs(pb->y - child->y);
                child->h = (D * (d1 + d2)) + ((D2 - (2 * D)) * imin(d1, d2));

                openSet[openSetSize++] = child;
            } else if (gScore < child->g) {
                bestGScore = TRUE;
            }

            if (bestGScore) {
                child->g = gScore;
                child->f = child->g + child->h;
            }
        }
    }

    return NULL;
}

int pf_get_path_cell_count(As_Node path) {
    As_Node node = path;
    int count = 0;

    while (node != NULL) {
        count++;
        node = node->parent;
    }

    return count;
}

Coords* pf_reconstruct_path(As_Node path, int count) {
    Coords* cells = (Coords*)malloc(count * sizeof(COORDS));
    As_Node node = path;

    for (int i = 0; i < count; i++) {
        if (node == NULL) break;

        Coords coords = defaultCoords();
        coords->x = node->x;
        coords->y = node->y;

        cells[i] = coords;
        node = node->parent;
    }

    return cells;
}

// int main() {
//     int grid[10][10] = {
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//         // {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
//     };
//     // int** gridp = grid;

//     Coords pa = defaultCoords();
//     pa->x = 1;
//     pa->y = 1;

//     Coords pb = defaultCoords();
//     pb->x = 8;
//     pb->y = 2;

//     // Coords* path = pathfind(grid, 10, 10, pa, pb);
//     As_Node path = pathfind(10, 10, grid, pa, pb, 0);
//     int path_cell_count = pf_get_path_cell_count(path);
//     Coords* path_cells = pf_reconstruct_path(path, path_cell_count);

//     for (int i = 0; i < path_cell_count; i++) {
//         printf("X: %d | Y: %d\n", path_cells[i]->x, path_cells[i]->y);
//     }
// }