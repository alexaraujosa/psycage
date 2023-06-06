#include "player.h"
#include "../../render/render.h"
#include "../../data/items.h"
#include "../../data/save.h"
#include <string.h>

#define MAX_CANDLE_FUEL 10
#define MAX_RADIUS_PRIEST 6
#define MAX_RADIUS_DETECTIVE 8
#define MAX_RADIUS_MERCENARY 5

#define MIN_RADIUS_PRIEST 2
#define MIN_RADIUS_DETECTIVE 4
#define MIN_RADIUS_MERCENARY 5

#define KILLS_TO_ULTIMATE 20
#define TIME_ULTIMATE 20

#define KILLS_TO_CHANGE_RADIUS 15

Clock clock_ultimate = NULL;

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
    player->last_direction = 0;
    player->class = Priest;
    player->radius = 0;
    player->sanity = 100;
    player->candle_fuel = 0;
    player->current_candle = 5;
    player->hasUltimate = 0;
    player->molotov = 0;
    player->trap = 0;

    player->item = get_random_item();

    return player;
}


void killCount(Player player, Entity entity) {
    if (entity->health == 0) {
        player->kills += 1;
        get_granade();
    }
    if(player->kills % KILLS_TO_CHANGE_RADIUS == 0)
        verifyPlayerRadius();
    if(player->kills % KILLS_TO_ULTIMATE == 0)
        g_gamestate->player->hasUltimate = 1;
}

void killXp(Player player) {
    player->kills = player->xp;
}

void destroyPlayer(Player player) {
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
    if(g_gamestate->player->cheats->vision == 1) return;
    
    switch(g_gamestate->player->class) {
        case Priest: {
            if(g_gamestate->player->radius < MAX_RADIUS_PRIEST) g_gamestate->player->radius++;
            break;
        }
        case Detective: {
            if(g_gamestate->player->radius > MIN_RADIUS_DETECTIVE) g_gamestate->player->radius--;
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
    
    for (int y = 0; y < HEIGHT-1; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == 0) { 
                if(left_closest_x == x && left_i < 20) {      
                    left_multi_closest_y[left_i++] = y;
                }

                if (left_closest_x > x) {
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
    int right_multi_closest_y[20] = { 0 };
    int right_i = 0;
    
    for (int y = 0; y < HEIGHT-1; y++) {
        for (int x = WIDTH-1; x >= 0 ; x--){
            if (map[y][x] == 0){
                if(right_closest_x == x && right_i < 20) {  
                    right_multi_closest_y[right_i++] = y;
                }

                if (right_closest_x < x) {
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

    if (distance_right > distance_left) {
        int right_random_index = rand() % right_i;
        int closest_y = right_multi_closest_y[right_random_index];    
        
        player->entity->coords->x = right_closest_x;
        player->entity->coords->y = closest_y;  
    } else {
        int left_random_index = rand() % left_i;
        int closest_y = left_multi_closest_y[left_random_index];

        player->entity->coords->x = left_closest_x;
        player->entity->coords->y = closest_y;
    }
}

char* stringify_class(int class) {
    switch (class) {
        case Priest:
            return "Priest";
            break;
        case Detective:
            return "Detective";
            break;
        case Mercenary:
            return "Mercenery";
            break;
        default:
            return "<undefined>";
    }
}

int is_player_insane(Player player) {
    return player->sanity <= 0;
}

void restore_sanity(Player player, int sanity) {
    player->sanity += sanity;
    if (player->sanity > 100) player->sanity = 100;
}

void reduce_sanity(Player player, int sanity) {
    player->sanity -= sanity;
    if (player->sanity < -100) player->sanity = -100;
}

int get_player_radius(Player player) {
    return player->radius + get_candle_light(player);
}

int get_candle_light(Player player) {
    if (player->current_candle == 0) return 0;

    int max_class_radius = 1;
    switch (player->class) {
        case Priest:
            max_class_radius = MAX_RADIUS_PRIEST;
            break;
        case Detective:
            max_class_radius = MAX_RADIUS_PRIEST;
            break;
        case Mercenary:
            max_class_radius = MAX_RADIUS_MERCENARY;
            break;
    }

    return (player->current_candle * max_class_radius) / MAX_CANDLE_FUEL;
}

void add_candle_fuel(Player player, int fuel) {
    player->candle_fuel += fuel;
    // if (player->candle_fuel > MAX_CANDLE_FUEL) player->candle_fuel = MAX_CANDLE_FUEL;
}

void reduce_candle_fuel(Player player, int fuel) {
    player->candle_fuel -= fuel;
    if (player->candle_fuel < 0) player->candle_fuel = 0;
}

void add_to_current_candle(Player player, int fuel) {
    player->current_candle += fuel;
    if (player->current_candle > MAX_CANDLE_FUEL) player->current_candle = MAX_CANDLE_FUEL;
}

void reduce_from_current_candle(Player player, int fuel) {
    player->current_candle -= fuel;
    if (player->current_candle < 0) player->current_candle = 0;
}

void use_candle_fuel(Player player, int fuel) {
    if (player->candle_fuel < fuel) return;

    int maxAmount = MAX_CANDLE_FUEL - player->current_candle;
    int amount = imin(fuel, maxAmount);

    reduce_candle_fuel(player, amount);
    add_to_current_candle(player, amount);

    if (!is_player_insane(player)) {
        int sanity = (rand() % 15) + 5;
	    restore_sanity(g_gamestate->player, sanity);
    }
}

void init_ultimate_clocks() {
    clock_ultimate = defaultClock();
    clock_ultimate->maxTicks *= TIME_ULTIMATE;
    clock_ultimate->blocked = 1;
    addClock(clock_ultimate);

    return;
}

void ultimate_reset() {
    clock_ultimate->blocked = 1;
    clock_ultimate->ticks = 0;
    g_gamestate->player->hasUltimate = 0;

    return;
}

void ultimate_priest() {
    for(int i = 0 ; i < g_gamestate->mob_begin ; i++)
        killMob(g_gamestate->mobs[i]);

    g_gamestate->player->hasUltimate = 0;

    return;
}

void ultimate_detective() {
    for(int i = 0 ; i < g_gamestate->mob_begin ; i++)
        if(g_gamestate->mobs[i]->entity->coords->y != 0 && g_gamestate->mobs[i]->entity->coords->x != 5)
            visible[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 1;

    return;
}

void ultimate_mercenary() {
    healEntityUltimate(g_gamestate->player->entity, 1);

    return;
}

void ultimate_use() {
    if(g_gamestate->player->class == Priest)
        ultimate_priest();
    else
        clock_ultimate->blocked = 0;

    return;
}

void ultimate_checker() {
    if(clock_ultimate->ticks == TICKS_PER_SECOND*TIME_ULTIMATE - 1) {
        ultimate_reset();
    } else if (clock_ultimate->ticks > 0 && g_gamestate->player->hasUltimate == 1)
        switch(g_gamestate->player->class) {
            case Detective:
                ultimate_detective();
                break;
            case Mercenary:
                ultimate_mercenary();
                break;
        }
    
    return;
}

void get_granade() {

    int granade_selector = (rand()%2);
    if((granade_selector == 0 && g_gamestate->player->trap == 0) || (granade_selector == 1 && g_gamestate->player->molotov == 0)){
        int chance = (rand()%4);
        if(chance == 0 && granade_selector == 0)
            g_gamestate->player->trap = 1;
        else if(chance == 0 && granade_selector == 1) 
            g_gamestate->player->molotov = 1;
    }
    return;
}