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
#include "../entity/player/player.h"
#include "../render/render.h"
#include "../map/map.h"

// Import paths from main
extern char* BIN_PATH;
extern char ASSET_DIR[PATH_MAX];

typedef struct gamestate {
    Player player;         // Player structure
    int input_initialized; // Block user input
    int clock;             // General tick clock
    int block_clock;       // Stop clock execution
    Coords pointA;
    Coords pointB;
    int recalculate;
    int path_cell_count;
    Coords* path_cells;
} GAMESTATE, *Gamestate;

extern Gamestate g_gamestate;
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

#endif