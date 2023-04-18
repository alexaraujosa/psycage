#include "player.h"
#include <string.h>

Player defaultPlayer() {
    Player player = (Player)malloc(sizeof(PLAYER));
    if (player == NULL) return NULL;

    Entity entity = defaultEntity();
    if (entity == NULL) return NULL;
    
    player->entity = entity;

    return player;
}

void destroyPlayer(Player player) {
    destroyEntity(player->entity);
    free(player);

    return;
}