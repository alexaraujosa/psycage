#include "entity.h"
#include <stdio.h>

Entity defaultEntity() {
    Entity entity = (Entity)malloc(sizeof(ENTITY));
    if (entity == NULL) return NULL;
    entity->coords = defaultCoords();
    entity->maxHealth = 12;
    entity->health = 12;
    entity->armor = 0;
    entity->basedamage = 4;

    return entity;
}

void destroyEntity(Entity entity) {
    destroyCoords(entity->coords);
    free(entity);
}

void damageEntity(Entity entity, int hp) {
    if (entity->armor >= hp) {
        entity->armor -= hp;
    } else {
        // entity->armor = 0;

        if (entity->health >= (unsigned long long)hp) {
            hp -= entity->armor;

            entity->armor = 0;
            entity->health -= hp;
        } else {
            entity->health = 0;
            //falta colocar a mobKill aqui, isto faz com que o mob caso tenho vida 0 continue com AI
        }
    }
}

void healEntity(Entity entity, int hp) {
    entity->health += imin(hp, entity->maxHealth - entity->health);
}

void healEntityUltimate(Entity entity, int hp) {
    if(entity->maxHealth/2 + 1 > entity->health + imin(hp, entity->maxHealth - entity->health))
        entity->health += imin(hp, entity->maxHealth - entity->health);
}