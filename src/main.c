#include <stdio.h>
#include "map/map.h"
#include "entity/entity.h"
#include "entity/player/player.h"
#include "render/render.h"
#include "gameloop/gameloop.h"

int main() {
    printf("Hello, world!\n");

    // Coords test = defaultCoords();
    // printf("Default coords: %d %d\n", test->x, test->y);

    // Entity test = defaultEntity();
    // printf("Default coords: %d %d\n", test->coords->x, test->coords->y);
    // printf("Entity: %d %d\n", test->health, test->armor);

    Player test = defaultPlayer();
    printf("Player coords: %d %d\n", test->entity->coords->x, test->entity->coords->y);
    printf("Player 1: %d %d %d\n", test->entity->health, test->entity->armor, test->entity->maxHealth);

    damageEntity(test->entity, 1);
    printf("Player 2: %d %d\n", test->entity->health, test->entity->armor);
    healEntity(test->entity, 2);
    printf("Player 3: %d %d\n", test->entity->health, test->entity->armor);

    render();
    
    return 0;
}