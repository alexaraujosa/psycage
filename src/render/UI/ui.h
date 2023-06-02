#ifndef __RL_RENDER_UI_H
#define __RL_RENDER_UI_H

#include "../render.h"

extern int g_ui_size[2];

Menu getMenuCacheOrCreate(MenuId id);
void removeMenuCache(MenuId id);
void deleteMenuCache(MenuId id);
void hmrMenuCache(MenuId id);
void drawMenu(Menu menu);
void tick_menu(Menu menu);
void handle_menu_keybinds(Menu menu, int key);
void cleanup_menu(Menu menu);

void drawGameInterface();
void addConsoleMessage(int value, char* key);

#endif