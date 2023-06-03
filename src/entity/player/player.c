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
    
    DataItemNode item = (DataItemNode)malloc(sizeof(DataItemNode));
    if (item == NULL) return NULL;

    Cheats cheats = defaultCheats();
    if (cheats == NULL) return NULL;

    player->cheats = cheats;
    player->item = item;
    player->entity = entity;
    player->level = 0;
    player->kills = 0;
    player->xp = 0;
    player->class = Priest;
    player->radius = 0;

    player->item = get_random_item();

    return player;
}

void levelUp(Player player, Entity entity) {
    if (player->level <= 99) {
        if (player->xp % 5 == 0) {
            player->level += 1;
            player->xp = 0;
            player->entity->basedamage += 1;
            player->entity->maxHealth += 4;
            player->entity->health = player->entity->maxHealth;

            if (player->level <= 25) {
                entity->maxHealth += 4;
                entity->basedamage += 2;
             }
            if (player->level > 25 && player->level<= 50) {
                entity->maxHealth += 8;
                entity->basedamage += 4;
             }
            if (player->level > 50 && player->level<= 75) {
                entity->maxHealth += 12;
                entity->basedamage += 6;
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
    // for(int i = 1 ; i <= 3 ; i++)
    //     delete_Save(i);
    destroyCheats(player->cheats);
    destroyEntity(player->entity);
    free(player);

    return;
}

int getLastDirection(Player player) {  //gpt diz que é int
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
    if(g_gamestate->player->cheats->vision == 1)
        return;
    
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

void player_spawn(Player player, int **map, int HEIGHT, int WIDTH) {

    // Spawn player in the closest position on the left
    int left_closest_x = 1000;
    int left_multi_closest_y[20] = {0};
    int left_i = 0;
    
    for (int y = 0; y < HEIGHT-1; y++){
        
        for (int x = 0; x < WIDTH; x++){
            
            if (map[y][x] == 0){
                
                if(left_closest_x == x && left_i < 20){
                    
                    left_multi_closest_y[left_i++] = y;
                }
                if (left_closest_x > x){

                    left_closest_x = x;
                    
                    for (int j = 0; j < left_i; j++){
                        left_multi_closest_y[j] = 0;
                    }
                    
                    left_multi_closest_y[0] = y;
                    left_i = 1;
                }

                break;
            }
        }
    }

    // Spawn player in the closest position on the right
    int right_closest_x = 0;
    int right_multi_closest_y[20] = {0};
    int right_i = 0;
    
    for (int y = 0; y < HEIGHT-1; y++){
        
        for (int x = WIDTH-1; x >= 0 ; x--){
            
            if (map[y][x] == 0){
                
                if(right_closest_x == x && right_i < 20){
                    
                    right_multi_closest_y[right_i++] = y;
                }
                if (right_closest_x < x){

                    right_closest_x = x;
                    
                    for (int j = 0; j < right_i; j++){
                        
                        right_multi_closest_y[j] = 0;
                    }
                    
                    right_multi_closest_y[0] = y;
                    right_i = 1;
                }

                break;
            }
        }
    }

    // Chooses the closest spawn location to a border (right or left)
    int distance_right = right_closest_x; 
    int distance_left = WIDTH - left_closest_x;

    if (distance_right > distance_left){

        int right_random_index = rand() % right_i;
        int closest_y = right_multi_closest_y[right_random_index];    
        
        player->entity->coords->x = right_closest_x;
        player->entity->coords->y = closest_y;  
    }
    else{

        int left_random_index = rand() % left_i;
        int closest_y = left_multi_closest_y[left_random_index];

        player->entity->coords->x = left_closest_x;
        player->entity->coords->y = closest_y;
    }
}