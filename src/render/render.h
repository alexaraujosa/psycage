/***************************************************************
 * RENDER MODULE                                               *
 *                                                             *
 * This module handles the rendering of all parts of the game. *
 ***************************************************************/

#ifndef __RL_RENDER_H
#define __RL_RENDER_H

// #include <locale.h>
#include <ncurses.h>
#include <panel.h>
#include "common.h"
#include "UI/menus/menulist.h"
#include "UI/ui.h"
#include "../gameloop/gameloop.h"
#include "../map/map_render.h"
#include "../src/data/locale.h"

typedef struct gamestate GAMESTATE, *Gamestate;

#define MENU_STACK_MAX 5

typedef struct renderstate {
    WINDOW* wnd;
    int nrows;
    int ncols;
    int activeMenus;
    int language;   // 0 - en_US || 1 - pt_PT
    Menu menus[MENU_STACK_MAX];
} RENDERSTATE, *Renderstate;

enum colors{
    // Colors
    GREY = 8,
    DARK_GREY,
    BROWN,
    DARK_RED,
    DARK_GREEN,
    DARK_DARK_GREY,
    LIGHT_GREY,
    GREEN,
    BLACK,
    WHITE,
    ORANGE,
    LIGHT_ORANGE,
    DARK_ORANGE,
    LIGHTPLUS_GREY,
    YELLOW_ORANGE,
    DARKPLUS_GREY,
    // Dungeon
    DUNGEON_BLOOD,
    DUNGEON_WALLS,
    DUNGEON_FLOOR,
    // Sewers
    SEWERS_BLOOD,
    SEWERS_FLOOR,
    SEWERS_WALLS,
    NOTHING,
    // Asylum
    ASYLUM_BLOOD,
    ASYLUM_WALLS,
    ASYLUM_FLOOR,
    // Player
    WHITE_PLAYER,
    YELLOW_PLAYER,
    BLUE_PLAYER,
    // Ascii
    ORANGE_LOGO,
    LIGHT_ORANGE_LOGO,
    DARK_ORANGE_LOGO,
    LIGHTPLUS_GREY_LOGO,
    YELLOW_ORANGE_LOGO,
    DARKPLUS_GREY_LOGO,
    // TEST
    GREEN_BG,
    RED_BG,
    YELLOW_BG
};

enum languages{
    en_US,
    pt_PT
};


// Global Renderstate
extern Renderstate g_renderstate;

/* FUNCTION SIGNATURES */

// External API
Renderstate init_render();
void render(Gamestate gs);

// Menu functions
Menu getActiveMenu();
Menu displayMenu(MenuId menu);
int closeMenu(MenuId menu);
int isMenuDisplayed(MenuId menu);
int isInMenu();
void _removeMenu(MenuId menu);

// Renderers
void render_game(Gamestate gs);
void render_menu(Gamestate gs);


#endif