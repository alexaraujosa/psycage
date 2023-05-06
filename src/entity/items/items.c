#include "items.h"
#include <string.h>

Item defaultItem() {
    Item item = (Item)malloc(sizeof(Item));
    if (item == NULL) return NULL;

    Entity entity = defaultEntity();
    if (entity == NULL) return NULL;

    item->entity = entity;
    if (item->plusdamage <= 5) item->plusdamage = item->normal;
    if (item->plusdamage > 5 && item->plusdamage <= 10) item->plusdamage = item->rare;
    if (item->plusdamage > 10) item->plusdamage = item->legendery;
    
    return item;
}

void destroyItem(Item item) {
    free(item->entity);
    free(item);
}