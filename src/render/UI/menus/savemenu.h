#ifndef __RL_RENDER_UI_MENU_SM_H
#define __RL_RENDER_UI_MENU_SM_H

#include "../ui.h"
#include "../../../data/save.h"

void draw_SaveMenu(Menu menu);
void tick_SaveMenu();
void handle_SaveMenu_keybinds(int key);
void draw_SaveInfo(Menu menu);
void cleanup_SaveMenu();

#endif