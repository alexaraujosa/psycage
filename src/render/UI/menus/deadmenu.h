#ifndef __RL_RENDER_UI_MENU_DM_H
#define __RL_RENDER_UI_MENU_DM_H

#include "../ui.h"
#include "util/ncurses.h"

void draw_DeadMenu(Menu menu);
void tick_DeadMenu();
void handle_DeadMenu_keybinds(int key);
void cleanup_dead_menu();

#endif