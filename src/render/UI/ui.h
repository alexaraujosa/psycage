#ifndef __RL_RENDER_UI_H
#define __RL_RENDER_UI_H

#include "../render.h"

extern int g_ui_size[2];

void drawMenu(Menu menu);
void tick_menu(Menu menu);
void handle_menu_keybinds(Menu menu, int key);
void drawGameInterface();
void addConsoleMessage(int value, char* key);

#endif