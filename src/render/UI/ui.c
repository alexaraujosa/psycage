#include "ui.h"
#include "menus/mainmenu.h"

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
        default:
            break;
    }
}

void handle_menu_keybinds(Menu menu, int key) {
    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            handle_MainMenu_Keybinds(key);
            break;
        }
        default:
            break;
    }
}