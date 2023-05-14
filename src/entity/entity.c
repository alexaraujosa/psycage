#include "entity.h"
#include "../render/render.h"
#include <stdio.h>

Entity defaultEntity() {
    Entity entity = (Entity)malloc(sizeof(ENTITY));
    if (entity == NULL) return NULL;
    entity->class = 0;
    entity->coords = defaultCoords();
    entity->maxHealth = 5;
    entity->health = 1;
    entity->armor = 0;
    entity->basedamage = 2;
    entity->damage = 1 + 1;

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

void entityIsDead(Entity entity) {
    if (entity->health == 0) entity->dead = 1;
    else entity->dead = 0;
}

char* getClassInterface(int classe) {

    switch(classe) {

        case Priest:
            return get_localized_string(g_renderstate->language, "user.interface.stats.priest");

        case Detective:
            return get_localized_string(g_renderstate->language, "user.interface.stats.detective");
        
        case Mercenary:
            return get_localized_string(g_renderstate->language, "user.interface.stats.mercenary");

    }


    return;
}