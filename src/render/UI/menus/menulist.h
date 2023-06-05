#ifndef __RL_RENDER_UI_MENULIST_H
#define __RL_RENDER_UI_MENULIST_H

#include <panel.h>

typedef enum {
    MENU_NONE,
    MENU_MAIN_MENU,
    MENU_DIALOG,
    MENU_OPTIONS,
    MENU_OPTIONS_KEYBINDER,
    MENU_PAUSE,
    MENU_DEAD,
    MENU_CHARACTERS,
    MENU_CHARACTERS_INFO,
    MENU_SAVE,
    MENU_SAVE_SLOT,
    MENU_CONSOLE,
    __MENU_COUNT
} MenuId;

static inline char* stringify_menu_id(MenuId id) {
    switch (id) {
        case MENU_NONE:
            return "MENU_NONE\0";
            break;
        case MENU_MAIN_MENU:
            return "MENU_MAIN_MENU\0";
            break;
        case MENU_DIALOG:
            return "MENU_DIALOG\0";
            break;
        case MENU_OPTIONS:
            return "MENU_OPTIONS\0";
            break;
        case MENU_OPTIONS_KEYBINDER:
            return "MENU_OPTIONS_KEYBINDER\0";
            break;
        case MENU_PAUSE:
            return "MENU_PAUSE\0";
            break;
        case MENU_DEAD:
            return "MENU_DEAD\0";
            break;
        case MENU_CHARACTERS:
            return "MENU_CHARACTERS\0";
            break;
        case MENU_CHARACTERS_INFO:
            return "MENU_CHARACTERS_INFO\0";
            break;
        case MENU_SAVE:
            return "MENU_SAVE\0";
            break;
        case MENU_SAVE_SLOT:
            return "MENU_SAVE_SLOT\0";
            break;
        case MENU_CONSOLE:
            return "MENU_CONSOLE\0";
            break;
        default:
            return "<unknown>\0";
            break;
    }
}

typedef struct menu {
    WINDOW* wnd;
    PANEL* panel;
    int active;
    int valid;
    MenuId id;
} MENU, *Menu;

#endif