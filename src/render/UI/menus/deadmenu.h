/***************************************************************
 * DEAD MENU                                                   *
 *                                                             *
 * This menu handles the UI for the player's death.            *
 *                                                             *
 * NOTE: Oh, look. That's what I want to be when I grow up!    *
 ***************************************************************/

#ifndef __RL_RENDER_UI_MENU_DM_H
#define __RL_RENDER_UI_MENU_DM_H

#include "../ui.h"
#include "util/ncurses.h"

void draw_DeadMenu(Menu menu);
void tick_DeadMenu();
void handle_DeadMenu_keybinds(int key);
void cleanup_dead_menu();

#endif