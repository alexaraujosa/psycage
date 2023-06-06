#ifndef __RL_RENDER_UI_MENU_OM_H
#define __RL_RENDER_UI_MENU_OM_H

#include "../ui.h"

#define BOTOES_KEYBINDER 11
#define ESPACAMENTO_KEYBINDER 2
#define LARGURA_OPTIONS_KEYBINDER g_renderstate->ncols/4
#define ALTURA_OPTIONS_KEYBINDER g_renderstate->nrows/4

void draw_OptionsMenu(Menu menu);
void tick_OptionsMenu();
void handle_OptionsMenu_keybinds(int key);
void cleanup_options_menu();
void draw_OptionsKeybinderMenu(Menu menu);
void tick_OptionsKeybinderMenu();
void handle_OptionsKeybinderMenu_keybinds(int key);
void cleanup_options_keybinder();

#endif