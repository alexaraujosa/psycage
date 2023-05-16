#include "ai.h"

#define NUM_OBSTACLES 3
static int obstacles[NUM_OBSTACLES] = { 1, 3, 5 };

Mob defaultMob() {
    Mob mob = (Mob)malloc(sizeof(MOB));
    if (mob == NULL) return NULL;

    mob->entity = defaultEntity();
    mob->moveCooldown = 10;
    mob->lastMove = FALSE;
    mob->hasAI = TRUE;

    return mob;
}

int trackPlayer(Coords playerCoords, Mob mob, int** map, int width, int height) {
    map[mob->entity->coords->y][mob->entity->coords->x] = map_footprint[mob->entity->coords->y][mob->entity->coords->x];

	As_Node path = pathfind(
		height, 
		width, 
		map, 
		playerCoords, 
		mob->entity->coords, 
		obstacles,
		NUM_OBSTACLES
	);

    if (path != NULL) {
        int path_cell_count = pf_get_path_cell_count(path);
        Coords* path_cells = pf_reconstruct_path(path, path_cell_count);

        if (path_cell_count > 2) { // Points not adjacent cardinally
            // asylum[mob->entity->coords->y][mob->entity->coords->x] = map_footprint[mob->entity->coords->y][mob->entity->coords->x];
            mob->entity->coords = path_cells[1];
            map[mob->entity->coords->y][mob->entity->coords->x] = 5;

            return 0;
        }

        map[mob->entity->coords->y][mob->entity->coords->x] = 5;

        return 1;
    }

    return 1;
}

int attemptMoveMob(Coords playerCoords, Mob mob, int** map, int width, int height) {
    if (!mob->hasAI) return 1;
    
    mob->lastMove++;

    if (mob->lastMove >= mob->moveCooldown) {
        mob->lastMove = 0;
        return trackPlayer(playerCoords, mob, map, width, height);
    }

    return 1;
}

int _isObstacle(int cell) {
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        if (cell == obstacles[i]) return 1;
    }

    return 0;
}

int addMobToMap(Mob mob, int** map, int width, int height) {
    int cellX = -1, cellY = -1;
    
    while ((cellX == -1 && cellY == -1) || _isObstacle(map[cellY][cellX])) {
        cellX = (int)floor(((double) rand() / RAND_MAX) * (width - 1));
        cellY = (int)floor(((double) rand() / RAND_MAX) * (height - 1));
        // printf(
        //     "X: %d | Y: %d | %d %d %f %f\n", 
        //     cellX, 
        //     cellY, 
        //     width - 1, 
        //     height - 1,
        //     ((double) rand() / RAND_MAX) * (width - 1),
        //     ((double) rand() / RAND_MAX) * (height - 1)
        // );
    }

    mob->entity->coords->x = cellX;
    mob->entity->coords->y = cellY;

    return 0;
}