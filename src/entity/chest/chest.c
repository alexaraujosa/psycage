#include "chest.h"
#include "../ai/ai.h"

#define NUM_OBSTACLES 3

Chest defaultChest() {
    Chest chest = (Chest)malloc(sizeof(Chest));
    if (chest == NULL) return NULL;
    
    chest->entity = defaultEntity();
    
    return chest;
}

int addChestToMap(Chest chest, int** map, int width, int height) {
    int cellX = -1, cellY = -1;
    
    while ((cellX == -1 && cellY == -1) || _isObstacle(map[cellY][cellX])) {
        cellX = (int)floor(((double) rand() / RAND_MAX) * (width - 1));
        cellY = (int)floor(((double) rand() / RAND_MAX) * (height - 1));
    }

    chest->entity->coords->x = cellX;
    chest->entity->coords->y = cellY;

    return 0;
}

void destroyItem(Chest chest) {
    free(chest->entity);
    free(chest);
}