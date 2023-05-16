#include "pathfind.h"

#define DIST 1
#define MIN_X 0
#define MIN_Y 0

#define NUM_POSITIONS 4
static int POSITIONS[NUM_POSITIONS][2] = { {0, 1}, {-1, 0}, {1, 0}, {0, -1} };

As_Node make_node(int x, int y, As_Node parent) {
    As_Node node = (As_Node)malloc(sizeof(AS_NODE));
    node->x = x;
    node->y = y;
    node->g = 0x7fffffff;
    node->closed = FALSE;
    node->parent = parent;
    return node;
}

static int equal_points(int pax, int pay, int pbx, int pby) {
    return pax == pbx && pay == pby;
}

static int point_inbounds(int g_height, int g_width, int px, int py) {
    int maxX = g_width - 1;
    int maxY = g_height - 1;

    return (px >= MIN_X && px <= maxX) && (py >= MIN_Y && py <= maxY);
}

static int valid_tile(int g_height, int g_width, int** grid, int* obstacles, int obsLen, int px, int py) {
    int isObstacle = FALSE;
    for (int i = 0; i < obsLen; i++) {
        if (grid[py][px] == obstacles[i]) {
            isObstacle = TRUE;
            break;
        }
    }

    return point_inbounds(g_height, g_width, px, py) && !isObstacle;
}

static As_Node getTileOrCreate(As_Node** grid, int x, int y, As_Node parent) {
    if (grid[y][x] != NULL) return grid[y][x];

    As_Node tile = make_node(x, y, parent);
    grid[y][x] = tile;

    return grid[y][x];
}

As_Node pathfind(int g_height, int g_width, int** grid, Coords pa, Coords pb, int* obstacles, int obsLen) {
    if (equal_points(pa->x, pa->y, pb->x, pb->y)) return NULL;

    As_Node** verifyGrid = malloc(g_height * sizeof(As_Node*));
    for (int i = 0; i < g_height; i++) {
        verifyGrid[i] = malloc(g_width * sizeof(As_Node));

        for (int j = 0; j < g_width; j++) {
            verifyGrid[i][j] = NULL;
        }
    }

    As_Node startNode = make_node(pa->x, pa->y, NULL);
    startNode->g = 0;
    verifyGrid[pa->y][pa->x] = startNode;

    As_Node* openSet = malloc(g_height * g_width * sizeof(As_Node));
    openSet[0] = startNode;
    int openSetLen = 1;

    As_Node* closedSet = malloc((g_height * g_width * NUM_POSITIONS) * sizeof(As_Node));
    int closedSetLen = 0;

    while (openSetLen > 0) {
        As_Node cur = openSet[0];
        int cind = 0;

        for (int i = 1; i < openSetLen; i++) {
            if (cur->g < openSet[i]->g) {
                cur = openSet[i];
                cind = i;
            }
        }

        if (equal_points(cur->x, cur->y, pb->x, pb->y)) {
            free(openSet);
            free(closedSet);

            for (int i = 0; i < g_height; i++) {
                free(verifyGrid[i]);
            }

            free(verifyGrid);

            return cur;
        }

        openSet[cind] = openSet[openSetLen - 1];
        openSetLen--;
        
        cur->closed = TRUE;

        if (closedSetLen < g_height * g_width) {
            closedSet[closedSetLen] = cur;
            closedSetLen++;
        }

        for (int i = 0; i < NUM_POSITIONS; i++) {
            int x = cur->x + POSITIONS[i][0];
            int y = cur->y + POSITIONS[i][1];
            if (!point_inbounds(g_height, g_width, x, y)) continue;
            if (!valid_tile(g_height, g_width, grid, obstacles, obsLen, x, y)) continue;

            As_Node neighbor = getTileOrCreate(verifyGrid, x, y, cur);
            if (neighbor->closed) continue;

            neighbor->g = cur->g + DIST;

            openSet[openSetLen] = neighbor;
            openSetLen++;
        }
    }

    free(openSet);
    free(closedSet);

    for (int i = 0; i < g_height; i++) {
        free(verifyGrid[i]);
    }

    free(verifyGrid);

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
    
//     int* grid_rows[10];
//     for (int i = 0; i < 10; i++) {
//         grid_rows[i] = grid[i];
//     }
//     int** gridp = grid_rows;

//     int obstacles[2] = {1, 3};

//     Coords pa = defaultCoords();
//     pa->x = 1;
//     pa->y = 1;

//     Coords pb = defaultCoords();
//     pb->x = 8;
//     pb->y = 2;

//     // Coords* path = pathfind(grid, 10, 10, pa, pb);
//     As_Node path = pathfind(10, 10, gridp, pa, pb, obstacles, 2);
//     int path_cell_count = pf_get_path_cell_count(path);
//     Coords* path_cells = pf_reconstruct_path(path, path_cell_count);

//     for (int i = 0; i < path_cell_count; i++) {
//         printf("X: %d | Y: %d\n", path_cells[i]->x, path_cells[i]->y);
//     }
// }