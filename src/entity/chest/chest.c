#include "chest.h"
#include <string.h>

Chest defaultChest() {
    Chest chest = (Chest)malloc(sizeof(Chest));
    if (chest == NULL) return NULL;

    Entity entity = defaultEntity();
    if (entity == NULL) return NULL;
    
    return chest;
}

void destroyItem(Chest chest) {
    free(chest->entity);
    free(chest);
}