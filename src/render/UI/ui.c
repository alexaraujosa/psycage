#include "ui.h"
#include "menus/mainmenu.h"
#include "menus/optionsmenu.h"
#include "menus/pausemenu.h"
#include "menus/dialog.h"
#include "menus/deadmenu.h"
#include "menus/charactersmenu.h"

int g_ui_size[2] = { 0 };

void drawMenu(Menu menu) {
    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL* panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            drawMainMenu(menu);
            break;
        }
        case MENU_DIALOG:
            WINDOW* win = newwin( 
                g_ui_size[0],
                g_ui_size[1],
                (g_renderstate->nrows / 2) - (g_ui_size[0] / 2), 
                (g_renderstate->ncols / 2) - (g_ui_size[1] / 2)
            );
            PANEL* panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            drawDialog(menu);
            break;
        case MENU_OPTIONS: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            drawOptionsMenu(menu);
            break;
        }
        case MENU_PAUSE: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            drawPauseMenu(menu);
            break;
        }
        case MENU_DEAD: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            drawDeadMenu(menu);
            break; 
        }
        case MENU_CHARACTERS: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            drawCharactersMenu(menu);
            break;
        }
        default:
            break;
    }
}

void tick_menu(Menu menu) {
    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            tick_MainMenu();
            break;
        }
        case MENU_DIALOG: {
            tick_dialog();
            break;
        }
        case MENU_OPTIONS: {
            tick_OptionsMenu();
            break;
        }
        case MENU_PAUSE: {
            tick_PauseMenu();
            break;
        }
        case MENU_DEAD: {
            tick_DeadMenu();
            break;
        }
        case MENU_CHARACTERS: {
            tick_CharactersMenu();
            break;
        }
        default:
            break;
    }
}

void handle_menu_keybinds(Menu menu, int key) {

    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            handle_MainMenu_keybinds(key);
            break;
        }
        case MENU_DIALOG: {
            handle_dialog_keybinds(key);
            break;
        }
        case MENU_OPTIONS: {
            handle_OptionsMenu_keybinds(key);
            break;
        }
        case MENU_PAUSE: {
            handle_PauseMenu_keybinds(key);
            break;
        }
        case MENU_DEAD: {
            handle_DeadMenu_keybinds(key);
            break;
        }
        case MENU_CHARACTERS: {
            handle_CharactersMenu_keybinds(key);
            break;
        }
        default:
            break;
    }
}