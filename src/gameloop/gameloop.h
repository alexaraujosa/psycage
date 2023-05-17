/***************************************************************
 * ENTITY MODULE                                               *
 *                                                             *
 * This module handles the entity behavior, both for player    *
 *   controlled characters, mobs, dropped items and all other  *
 *   miscelaneous entities                                     *
 ***************************************************************/

#ifndef __RL_GAMELOOP_H
#define __RL_GAMELOOP_H

#include <ncurses.h>
#include "common.h"
#include "util/filesystem.h"
#include "gameloop_common.h"
// #include "../entity/player/player.h"
// #include "../entity/ai/ai.h"
#include "../render/render.h"
#include "../map/map.h"
#include "../map/light.h"
#include "../entity/cheats/cheats.h"

#ifndef useconds_t
typedef unsigned int useconds_t;
#endif

// Import paths from main
extern char* BIN_PATH;
extern int BIN_PATH_LEN;
extern char ASSET_DIR[PATH_MAX];
extern FILE* dbgOut;

// typedef struct gamestate {
//     Player player;         // Player structure
//     Projectile projectile;
//     int input_initialized; // Block user input
//     int clock;             // General tick clock
//     int block_clock;       // Stop clock execution
//     Coords pointA;
//     Coords pointB;
//     int recalculate;
//     int path_cell_count;
//     Coords* path_cells;
// } GAMESTATE, *Gamestate;

extern Gamestate g_gamestate;
extern int find_map;

extern char* g_dialog_text;
extern char**** g_dialog_page_data;
extern int g_dialog_control[3];

/* FUNCTION SIGNATURES */
Gamestate init_gameloop();
void tick();
void handle_keybinds();
void game_keybinds(int key);
void menu_keybinds(int key);
void move_player(int dx, int dy);
void move_projectile(int dx, int dy);

#endif