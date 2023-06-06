/***************************************************************
 * PLAYER MODULE                                               *
 *                                                             *
 * This module handles the player behavior.                    *
 ***************************************************************/

#ifndef __RL_PLAYER_H
#define __RL_PLAYER_H

#include "../entity.h"
#include "../../data/items.h"
#include "../cheats/cheats.h"

#define is_player_dead(player) ((player)->entity->health <= 0)

typedef struct player {
    Entity entity;
    DataItemNode item;
    Cheats cheats;
    int level;
    int kills;
    int xp;
    int last_direction;
    int class;
    int radius;
    int sanity;
    int candle_fuel;
    int current_candle;
    int hasUltimate;
} PLAYER, *Player;

enum Classes {
    Priest,
    Detective,
    Mercenary
};

// #define PLAYER_SIZE ENTITY_SIZE

/* FUNCTION SIGNATURES */
Player defaultPlayer();
void destroyPlayer(Player player);
char* getClassInterface(int classe);
void verifyPlayerRadius();
void player_spawn(Player player, int **map, int HEIGHT, int WIDTH);
char* stringify_class(int class);
int is_player_insane(Player player);
void restore_sanity(Player player, int sanity);
void reduce_sanity(Player player, int sanity);
int get_player_radius(Player player);
int get_candle_light(Player player);
void add_candle_fuel(Player player, int fuel);
void reduce_candle_fuel(Player player, int fuel);
void add_to_current_candle(Player player, int fuel);
void reduce_from_current_candle(Player player, int fuel);
void use_candle_fuel(Player player, int fuel);
void init_ultimate_clocks();
void ultimate_use();
void ultimate_reset();
void ultimate_detective();
void ultimate_checker();
void killCount(Player player, Entity entity);

#endif
