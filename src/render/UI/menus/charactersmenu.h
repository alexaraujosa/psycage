#ifndef __RL_RENDER_UI_MENU_CM_H
#define __RL_RENDER_UI_MENU_CM_H

#include "../ui.h"

#define BOTOES_CHARACTERS 3
#define ESPACAMENTO_CHARACTERS 2

void draw_CharactersMenu(Menu menu);
void draw_CharactersInfo(Menu menu);
void tick_CharactersMenu();
void handle_CharactersMenu_keybinds(int key);

#endif