#ifndef __RL_RENDER_UI_MENU_OM_H
#define __RL_RENDER_UI_MENU_OM_H

#include "../ui.h"

void draw_OptionsMenu(Menu menu);
void tick_OptionsMenu();
void handle_OptionsMenu_keybinds(int key);
void cleanup_options_menu();

#endif