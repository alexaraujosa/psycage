/***************************************************************
 * CONSOLE MENU                                                *
 *                                                             *
 * This menu handles the entirety of the in-game console.      *
 *                                                             *
 * NOTE: Don't worry, I don't have the patience to read all    *
 * this either.                                                *
 ***************************************************************/

#ifndef __RL_RENDER_UI_MENU_CONSOLE_H
#define __RL_RENDER_UI_MENU_CONSOLE_H

#include <unistd.h>
#include <time.h>
#include "../ui.h"
#include "util/ncurses.h"
#include "../../../vendor/tiny-regex/re.h"

extern int menu_wins;

void draw_ConsoleMenu(Menu menu);
void tick_ConsoleMenu();
void handle_ConsoleMenu_keybinds(int key);
void cleanup_ConsoleMenu();

#endif