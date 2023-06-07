/***************************************************************
 * PAUSE MENU                                                  *
 *                                                             *
 * This menu handles the UI for the pause screen.              *
 *                                                             *
 * NOTE: Can I get this thing for life?                        *
 ***************************************************************/

#ifndef __RL_RENDER_UI_MENU_PM_H
#define __RL_RENDER_UI_MENU_PM_H

#include "../ui.h"
#include "util/ncurses.h"

void draw_PauseMenu(Menu menu);
void tick_PauseMenu();
void handle_PauseMenu_keybinds(int key);
void cleanup_pause_menu();

#endif