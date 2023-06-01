#ifndef __RL_RENDER_UI_MENU_CM_H
#define __RL_RENDER_UI_MENU_CM_H

#include "../ui.h"
#include "util/ncurses.h"

#define BOTOES_CHARACTERS 3
#define ESPACAMENTO_CHARACTERS 2
#define LARGURA_CHARACTERS_INFO g_renderstate->ncols/4
#define ALTURA_CHARACTERS_INFO g_renderstate->nrows/3 + ESPACAMENTO_CHARACTERS + BOTOES_CHARACTERS*2 + 2

void draw_CharactersMenu(Menu menu);
void draw_CharactersInfo(Menu menu);
void tick_CharactersMenu();
void handle_CharactersMenu_keybinds(int key);
void cleanup_characters_menu();

#endif