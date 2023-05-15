#include "player.h"
#include "../../render/render.h"
#include "../../data/items.h"
#include <string.h>

Player defaultPlayer() {
    Player player = (Player)malloc(sizeof(PLAYER));
    if (player == NULL) return NULL;

    Entity entity = defaultEntity();
    if (entity == NULL) return NULL;
    
    player->entity = entity;
    player->level = 0;
    player->kills = 0;
    player->xp = 0;
    player->cheats = 0;
    player->class = 0;

    return player;
}

void levelUp(Player player, Entity entity) {
    if (player->level <= 99) {
        if (player->xp % 5 == 0) {
            player->level += 1;
            player->xp = 0;
            player->entity->basedamage += 1;
            player->entity->maxHealth += 4;
            player->entity->health == player->entity->maxHealth;
            if (player->level <= 25) {
                entity->maxHealth += 8;
                entity->basedamage += 4;
             }
        }
    }
}

void killCount(Player player, Entity entity) {
    if (entity->health == 0) {
        player->kills += 1;
    }
}

void killXp(Player player) {
    player->kills = player->xp;
}

void destroyPlayer(Player player) {
    destroyEntity(player->entity);
    free(player);

    return;
}

void getLastDirection(Player player) {  //gpt diz que é int
    return player->last_direction;
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