#ifndef __RL_RENDER_UI_MENU_SM_H
#define __RL_RENDER_UI_MENU_SM_H

#include "../ui.h"

void drawSaveMenu(Menu menu);
void tick_SaveMenu();
void handle_SaveMenu_keybinds(int key);
void drawSaveInfo(Menu menu);

#endif