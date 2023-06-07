/***************************************************************
 * SAVE MENU                                                   *
 *                                                             *
 * This menu handles the UI for the save screen.               *
 *                                                             *
 * NOTE: You can save your game, but nothing can save you from *
 * the one that's standing behind you.                         *
 ***************************************************************/

#ifndef __RL_RENDER_UI_MENU_SM_H
#define __RL_RENDER_UI_MENU_SM_H

#include "../ui.h"
#include "./dialog.h"
#include "../../../data/save.h"
#include "util/ncurses.h"

void draw_SaveMenu(Menu menu);
void tick_SaveMenu();
void handle_SaveMenu_keybinds(int key);
void draw_SaveInfo(Menu menu);
void cleanup_SaveMenu();

#endif