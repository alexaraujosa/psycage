#include "entity.h"
#include <stdio.h>

Entity defaultEntity() {
    Entity entity = (Entity)malloc(sizeof(ENTITY));
    if (entity == NULL) return NULL;

    entity->coords = defaultCoords();
    entity->maxHealth = 5;
    entity->health = 1;
    entity->armor = 0;

    return entity;
}

void destroyEntity(Entity entity) {
    free(entity->coords);
    free(entity);
}

void damageEntity(Entity entity, int hp) {
    if (entity->armor >= hp) {
        entity->armor -= hp;
    } else {
        entity->armor = 0;

        if (entity->health >= hp) {
            hp -= entity->armor;
            entity->health -= hp;
        } else {
            entity->health = 0;
        }
    }
}

void healEntity(Entity entity, int hp) {
    entity->health += imin(hp, entity->maxHealth - entity->health);
}