#include "player.h"
#include "../../render/render.h"
#include "../../data/items.h"
#include "../../data/save.h"
#include <string.h>

#define MAX_RADIUS_PRIEST 6
#define MIN_RADIUS_DETECTIVE 4
#define KILLS_TO_CHANGE_RADIUS 15



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
    player->class = Priest;
    player->radius = 0;

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
    if(player->kills % KILLS_TO_CHANGE_RADIUS == 0)
        verifyPlayerRadius();
}

void killXp(Player player) {
    player->kills = player->xp;
}

void destroyPlayer(Player player) {
    for(int i = 1 ; i <= 3 ; i++)
        delete_Save(i);
    
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

    return NULL;
}

void verifyPlayerRadius() {

    switch(g_gamestate->player->class) {

        case Priest: {

            if(g_gamestate->player->radius < MAX_RADIUS_PRIEST)
                g_gamestate->player->radius++;

            break;

        }

        case Detective: {

            if(g_gamestate->player->radius > MIN_RADIUS_DETECTIVE)
                g_gamestate->player->radius--;

            break;

        }
    }

    return;
}