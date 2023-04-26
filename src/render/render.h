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

typedef struct gamestate GAMESTATE, *Gamestate;

#define MENU_STACK_MAX 5

typedef struct renderstate {
    WINDOW* wnd;
    int nrows;
    int ncols;

    int activeMenus;
    Menu menus[MENU_STACK_MAX];
} RENDERSTATE, *Renderstate;

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