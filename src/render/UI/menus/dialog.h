/***************************************************************
 * DIALOG MENU                                                 *
 *                                                             *
 * This menu handles the UI for the popup dialog windows.      *
 *                                                             *
 * NOTE: A wise man once said that all problems should be      *
 * solved with dialog. That's why I keep my baseball bat named *
 * 'dialog' near me at all times.                              *
 ***************************************************************/

#ifndef __RL_RENDER_UI_MENU_D_H
#define __RL_RENDER_UI_MENU_D_H

#include "../ui.h"
#include "common.h"
#include "util/string.h"
#include "util/ncurses.h"

extern char* g_dialog_text;
extern char**** g_dialog_page_data;
extern int g_dialog_control[4];
extern int (*g_dialog_keybinds)(int key);

/* FUNCTION SIGNATURES */
void draw_Dialog(Menu menu);
void tick_Dialog();
void handle_Dialog_keybinds(int key);
void cleanup_Dialog();
int calculate_dialog_metadata(char* text, char**** ret);

#endif