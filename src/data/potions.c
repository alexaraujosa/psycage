#include "potions.h"

#define TEMPO_POTION_STRENGTH 20

Clock clock_strength = NULL;

void init_potions_clock() {
    clock_strength = defaultClock();
    clock_strength->maxTicks *= TEMPO_POTION_STRENGTH;
    clock_strength->blocked = 1;
    addClock(clock_strength);

    return;
}


void create_potion() {

    int cellX = -1, cellY = -1;
    
    while ((cellX == -1 && cellY == -1) || _isObstacle(map[cellY][cellX])) {
        cellX = (int)floor(((double) rand() / RAND_MAX) * (LARGURA_JOGO - 1));
        cellY = (int)floor(((double) rand() / RAND_MAX) * (ALTURA_JOGO - 1));
    }

    map[cellY][cellX] = 7;

    return;
}


void use_random_potion() {

    int temporary = (rand() % 2);

    if(temporary) {
        g_gamestate->player->entity->basedamage *= 2;
        g_gamestate->potion_strength = 1;
        clock_strength->blocked = 0;
    }
    else
        healEntity(g_gamestate->player->entity, g_gamestate->player->entity->maxHealth/3);
    
    map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] = 0;

    return;
}


void remove_potion_strength_effect() {

    g_gamestate->player->entity->basedamage /= 2;
    g_gamestate->potion_strength = 0;
    clock_strength->blocked = 1;
    clock_strength->ticks = 0;

    return;
}


void potion_checker() {

    if(clock_strength->ticks == TICKS_PER_SECOND*TEMPO_POTION_STRENGTH - 1)
        remove_potion_strength_effect();
    
    return;
}