#include "ui.h"
#include "util/ncurses.h"
#include "../../entity/cheats/cheats.h"
#include "common.h"
#include "menus/mainmenu.h"
#include "menus/optionsmenu.h"
#include "menus/pausemenu.h"
#include "menus/deadmenu.h"
#include "menus/dialog.h"
#include "menus/charactersmenu.h"
#include "menus/savemenu.h"
#include "menus/console.h"

//#define LARGURA_RETANGULO 52
#define LARGURA_RETANGULO g_renderstate->ncols/3
#define ESTATISTICAS_TOTAL 11
#define ESTATISTICAS_ESQUERDA 6

int g_ui_size[2] = { 0 };

int menu_wins = 0;
static Menu menuCache[__MENU_COUNT];

Menu getMenuCacheOrCreate(MenuId id) {
    debug_file(dbgOut, 1, "Verifying Menu Cache for menu %s.\n", stringify_menu_id(id));

    if (menuCache[id] != NULL) {
        debug_file(dbgOut, 1, "- Menu Cache hit. Returning cached element.\n");
        return menuCache[id];
    } else {
        debug_file(dbgOut, 1, "- Menu Cache miss. Creating new element.\n");
        Menu menu = (Menu)malloc(sizeof(MENU));
        menu->wnd = NULL;
        menu->id = id;
        menu->active = 1;
        menu->valid = 1;

        menu_wins++;

        switch (id) {
            case MENU_NONE: break;
            case MENU_MAIN_MENU: {
                WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
                PANEL* panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_DIALOG: {
                WINDOW* win = newwin( 
                    g_ui_size[0],
                    g_ui_size[1],
                    (g_renderstate->nrows / 2) - (g_ui_size[0] / 2), 
                    (g_renderstate->ncols / 2) - (g_ui_size[1] / 2)
                );
                PANEL* panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_OPTIONS: {
                WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_OPTIONS_KEYBINDER: {
                WINDOW* win = newwin(yMAX/2, xMAX/2, ALTURA_OPTIONS_KEYBINDER, LARGURA_OPTIONS_KEYBINDER);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_PAUSE: {
                WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_DEAD: {
                WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break; 
            }
            case MENU_CHARACTERS: {
                WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_CHARACTERS_INFO: {
                WINDOW* win = newwin(yMAX/3, xMAX/2, ALTURA_CHARACTERS_INFO, LARGURA_CHARACTERS_INFO);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_SAVE: {
                WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_SAVE_SLOT: {
                WINDOW* win = newwin(g_renderstate->nrows/3, g_renderstate->ncols/4, g_renderstate->nrows/2, 3*g_renderstate->ncols/8);
                PANEL * panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            case MENU_CONSOLE: {
                WINDOW* win = newwin(g_renderstate->nrows - 6, g_renderstate->ncols - 6, 3, 3);
                // WINDOW* win = newwin(10, 10, 3, 3);
                PANEL* panel = new_panel(win);
                menu->wnd = win;
                menu->panel = panel;
                break;
            }
            default:
                menu_wins--;
                return NULL;
                break;
        }

        menuCache[id] = menu;
        return menu;
    }
}

void removeMenuCache(MenuId id) {
    debug_file(dbgOut, 1, "Attempting to destroy menu cache entry %s.\n", stringify_menu_id(id));
    if (menuCache[id] != NULL) {
        debug_file(dbgOut, 1, "- Menu Cache hit. Marking for destruction.\n");

        menuCache[id]->valid = 0;

        debug_file(dbgOut, 1, "- Menu marked for destruction.\n");
    } else {
        debug_file(dbgOut, 1, "- Menu Cache miss. Skipping.\n");
    }
}

void deleteMenuCache(MenuId id) {
    debug_file(dbgOut, 1, "Attempting to delete menu cache entry %s.\n", stringify_menu_id(id));

    if (menuCache[id] == NULL) {
        debug_file(dbgOut, 1, "Menu cache entry not present. Skipping.\n");
        return;
    }

    del_panel(menuCache[id]->panel);
    delwin(menuCache[id]->wnd);
    free(menuCache[id]);

    menuCache[id] = NULL;
    debug_file(dbgOut, 1, "Menu cache entry deleted. Skipping.\n");
}

void hmrMenuCache(MenuId id) {
    debug_file(dbgOut, 1, "Reloading menu cache entry %s.\n", stringify_menu_id(id));

    
    deleteMenuCache(id);
    Menu new_menu = getMenuCacheOrCreate(id);

    if (new_menu != NULL) {
        for (int i = 0; i < MENU_STACK_MAX; i++) {
            if (g_renderstate->menus[i] == NULL) break;

            if (g_renderstate->menus[i]->id == id) {
                g_renderstate->menus[i] = new_menu;
                break;
            }
        }
    }
}

void drawMenu(Menu menu) {
    if (menu == NULL) return;

    wclear(menu->wnd);

    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            draw_MainMenu(menu);
            break;
        }
        case MENU_DIALOG: {
            draw_Dialog(menu);
            break;
        }
        case MENU_OPTIONS: {
            draw_OptionsMenu(menu);
            break;
        }
        case MENU_OPTIONS_KEYBINDER: {
            draw_OptionsKeybinderMenu(menu);
            break;
        }
        case MENU_PAUSE: {
            draw_PauseMenu(menu);
            break;
        }
        case MENU_DEAD: {
            draw_DeadMenu(menu);
            break; 
        }
        case MENU_CHARACTERS: {
            draw_CharactersMenu(menu);
            break;
        }
        case MENU_CHARACTERS_INFO: {
            draw_CharactersInfo(menu);
            break;
        }
        case MENU_SAVE: {
            draw_SaveMenu(menu);
            break;
        }
        case MENU_SAVE_SLOT: {
            draw_SaveInfo(menu);
            break;
        }
        case MENU_CONSOLE: {
            draw_ConsoleMenu(menu);
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
            tick_Dialog();
            break;
        }
        case MENU_OPTIONS: {
            tick_OptionsMenu();
            break;
        }
        case MENU_OPTIONS_KEYBINDER: {
            tick_OptionsKeybinderMenu();
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
        case MENU_CHARACTERS_INFO: {
            tick_CharactersMenu();
            break;
        }
        case MENU_SAVE: {
            tick_SaveMenu();
            break;
        }
        case MENU_CONSOLE: {
            tick_ConsoleMenu();
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
            handle_Dialog_keybinds(key);
            break;
        }
        case MENU_OPTIONS: {
            handle_OptionsMenu_keybinds(key);
            break;
        }
        case MENU_OPTIONS_KEYBINDER: {
            handle_OptionsKeybinderMenu_keybinds(key);
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
        case MENU_CHARACTERS_INFO: {
            handle_CharactersMenu_keybinds(key);
            break;
        }
        case MENU_SAVE: {
            handle_SaveMenu_keybinds(key);
            break;
        }
        case MENU_SAVE_SLOT: {
            handle_SaveMenu_keybinds(key);
            break;
        }
        case MENU_CONSOLE: {
            handle_ConsoleMenu_keybinds(key);
            break;
        }
        default:
            break;
    }
}

void cleanup_menu(Menu menu) {
    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            cleanup_MainMenu();
            break;
        }
        case MENU_DIALOG: {
            cleanup_Dialog();
            break;
        }
        case MENU_OPTIONS: {
            cleanup_options_menu();
            break;
        }
        case MENU_OPTIONS_KEYBINDER: {
            cleanup_options_keybinder();
            break;
        }
        case MENU_PAUSE: {
            cleanup_pause_menu();
            break;
        }
        case MENU_DEAD: {
            cleanup_dead_menu();
            break;
        }
        case MENU_CHARACTERS: {
            cleanup_characters_menu();
            break;
        }
        case MENU_CHARACTERS_INFO: {
            cleanup_characters_menu();
            break;
        }
        case MENU_SAVE: {
            cleanup_SaveMenu();
            break;
        }
        case MENU_CONSOLE: {
            cleanup_ConsoleMenu();
            break;
        }
        default:
            break;
    }
}


// #define CLOCK_NUM 4
void drawGameInterface() {
    // static int dgiClocksBlocked = TRUE;
    // static int toggles[4] = { 1, 1, 1, 1 };
    // static Clock dgiClocks[CLOCK_NUM];
    // int reqTicks = 2; // (TICKS_PER_SECOND / 2) - 1;
    // static Colors borderColor = SANITY_EMPTY;

    // if (dgiClocks[0] == NULL) {
    //     for (int i = 0; i < CLOCK_NUM; i++) {
    //         dgiClocks[i] = defaultClock();
    //         // dgiClocks[i]->maxTicks = TICKS_PER_SECOND;
    //         dgiClocks[i]->maxTicks = reqTicks + 1;
    //         dgiClocks[i]->ticks = 0;
    //         dgiClocks[i]->blocked = TRUE;

    //         addClock(dgiClocks[i]);
    //     }
    // }

    // if (is_player_insane(g_gamestate->player)) {
    //     if (dgiClocksBlocked == TRUE) {
    //         for (int i = 0; i < CLOCK_NUM; i++) {
    //             dgiClocks[i]->blocked = FALSE;
    //         }
    //     }

    //     for (int i = 0; i < CLOCK_NUM; i++) {
    //         if (dgiClocks[i]->ticks == reqTicks) toggles[i] = !toggles[i];
    //     }
    // } else {
    //     for (int i = 0; i < 4; i++) {
    //         toggles[i] = 1;
    //     }
    // }

    static int dgiClockBlocked = TRUE;
    static int toggles[4] = { 1, 1, 1, 1 };
    static Clock dgiClock = NULL;
    int reqTicks = 2;
    static Colors borderColor = SANITY_EMPTY;

    if (dgiClock == NULL) {
        dgiClock = defaultClock();

        dgiClock->maxTicks = reqTicks + 1;
        dgiClock->ticks = 0;
        dgiClock->blocked = TRUE;

        addClock(dgiClock);
    }

    if (is_player_insane(g_gamestate->player)) {
        if (dgiClockBlocked == TRUE) {
            dgiClock->blocked = FALSE;
        }

        for (int i = 0; i < 4; i++) {
            if (dgiClock->ticks == reqTicks) toggles[i] = !toggles[i];
        }
    } else {
        for (int i = 0; i < 4; i++) {
            toggles[i] = 1;
        }
    }

    static char *stats[ESTATISTICAS_TOTAL] = {
        "user.interface.stats.class", 
        "user.interface.stats.level",
        "user.interface.stats.monsters",
        "user.interface.stats.hp",
        "user.interface.stats.armor", 
        "user.interface.stats.kills",
        "user.interface.stats.item",
        "user.interface.stats.item.damage",
        "user.interface.stats.candles",
        "user.interface.stats.trap",
        "user.interface.stats.molotov"
    };

    if (is_player_insane(g_gamestate->player)) {
        wattron(g_renderstate->wnd, COLOR_PAIR(borderColor));
        // debug_file(dbgOut, 2, "DGI Toggles: %d | %d | %d | %d\n", toggles[0], toggles[1], toggles[2], toggles[3]);
    }

    //Desenha o retângulo à esquerda do Logo
    if (toggles[0]) {
        if (is_player_insane(g_gamestate->player)) wattron(g_renderstate->wnd, COLOR_PAIR(borderColor));
        rectangle(g_renderstate->wnd, 0, 0, ALTURA_LOGO, g_renderstate->ncols/2 - LARGURA_LOGO/2 -1);
    } else {
        wattron(g_renderstate->wnd, COLOR_PAIR(EMPTY));
        rectangle(g_renderstate->wnd, 0, 0, ALTURA_LOGO, g_renderstate->ncols/2 - LARGURA_LOGO/2 -1);
        wattroff(g_renderstate->wnd, COLOR_PAIR(EMPTY));
    }

    //Desenha o retângulo à direita do Logo
    if (toggles[1]) {
        if (is_player_insane(g_gamestate->player)) wattron(g_renderstate->wnd, COLOR_PAIR(borderColor));
        rectangle(g_renderstate->wnd, 0, g_renderstate->ncols/2 + LARGURA_LOGO/2 +1, ALTURA_LOGO, g_renderstate->ncols);
    } else {
        wattron(g_renderstate->wnd, COLOR_PAIR(EMPTY));
        rectangle(g_renderstate->wnd, 0, g_renderstate->ncols/2 + LARGURA_LOGO/2 +1, ALTURA_LOGO, g_renderstate->ncols);
        wattroff(g_renderstate->wnd, COLOR_PAIR(EMPTY));
    }

    //Desenha o traço por baixo do Logo
    if (toggles[2]) {
        if (is_player_insane(g_gamestate->player)) wattron(g_renderstate->wnd, COLOR_PAIR(borderColor));
        rectangle(g_renderstate->wnd, ALTURA_LOGO, 0, g_renderstate->nrows-1, g_renderstate->ncols-1);
    } else {
        wattron(g_renderstate->wnd, COLOR_PAIR(EMPTY));
        rectangle(g_renderstate->wnd, ALTURA_LOGO, 0, g_renderstate->nrows-1, g_renderstate->ncols-1);
        wattroff(g_renderstate->wnd, COLOR_PAIR(EMPTY));
    }
    //Retângulo à volta da janela toda
    if (toggles[3]) {
        if (is_player_insane(g_gamestate->player)) wattron(g_renderstate->wnd, COLOR_PAIR(borderColor));
        rectangle(g_renderstate->wnd, 0, 0, g_renderstate->nrows-1, g_renderstate->ncols-1);
    } else { 
        wattron(g_renderstate->wnd, COLOR_PAIR(EMPTY));
        rectangle(g_renderstate->wnd, 0, 0, g_renderstate->nrows-1, g_renderstate->ncols-1);
        wattroff(g_renderstate->wnd, COLOR_PAIR(EMPTY));
    }
    
    if (is_player_insane(g_gamestate->player)) {
        wattroff(g_renderstate->wnd, COLOR_PAIR(borderColor));
    }

    //Print do Logo
    printer(g_renderstate->wnd, 0, g_renderstate->ncols/2 - LARGURA_LOGO/2+1);


/*************************************
 *                                   *
 *       Retângulo da Esquerda       *
 *                                   *
 ************************************/


    wattron(g_renderstate->wnd, A_BOLD);


    for(int i = 0 ; i < ESTATISTICAS_ESQUERDA/2 ; i++)
        mvwprintw(
                  g_renderstate->wnd,   
                  ALTURA_LOGO/3 + i,
                  2,
                  "%s",    get_localized_string(g_renderstate->language, stats[i])
                );
    
    for(int i = ESTATISTICAS_ESQUERDA/2, j = 0 ; i < ESTATISTICAS_ESQUERDA ; i++, j++)
        mvwprintw(
                  g_renderstate->wnd,   
                  ALTURA_LOGO/3 + j,
                  LARGURA_RETANGULO*0.6,
                  "%s",    get_localized_string(g_renderstate->language, stats[i])
                );


    wattroff(g_renderstate->wnd, A_BOLD);


/******************************************************
 *                                                    *
 *       Estatisticas no Retângulo da Esquerda        *
 *                                                    *
 *****************************************************/

    mvwprintw(g_renderstate->wnd, 
              ALTURA_LOGO/3 + 0,    
              4 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.class")),
              "%s      " , getClassInterface(g_gamestate->player->class)
            );


    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 1,    
            4 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.level")),
            "%d      ", g_gamestate->player->level
        );


    mvwprintw(g_renderstate->wnd,
              ALTURA_LOGO/3 + 2,
              4 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.monsters")),
              "%d      ",     g_gamestate->mob_count   
            );


    if(g_gamestate->player->cheats->godmode == 0) {

        mvwprintw(g_renderstate->wnd,
                ALTURA_LOGO/3 + 0, 
                LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.hp")),
                "%lld/%u    ", g_gamestate->player->entity->health, g_gamestate->player->entity->maxHealth
                );

    } else {

        mvwprintw(g_renderstate->wnd,
                ALTURA_LOGO/3 + 0, 
                LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.hp")),
                "%s", get_localized_string(g_renderstate->language, "user.interface.stats.god")
                );
    }


    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 1,    
            LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.armor")),
            "%d    ", g_gamestate->player->entity->armor
        );


    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 2,    
            LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.kills")),
            "%d    ", g_gamestate->player->kills
        );


/*************************************
 *                                   *
 *       Retângulo da Direita        *
 *                                   *
 ************************************/


    wattron(g_renderstate->wnd, A_BOLD);


    for(int i = ESTATISTICAS_ESQUERDA, j = 0 ; i < ESTATISTICAS_TOTAL ; i++, j++)
        mvwprintw(
                  g_renderstate->wnd,   
                  1 + j,
                  LARGURA_RETANGULO + LARGURA_LOGO + 4,
                  "%s",    get_localized_string(g_renderstate->language, stats[i])
                );

    if(g_gamestate->potion_strength == 1)
        mvwprintw(g_renderstate->wnd,
                  1 + 5,
                  LARGURA_RETANGULO + LARGURA_LOGO + 4,
                  "%s  ",     get_localized_string(g_renderstate->language, "user.interface.stats.potion.on")
                );
    else
        mvwprintw(g_renderstate->wnd,
                  1 + 5,
                  LARGURA_RETANGULO + LARGURA_LOGO + 4,
                  "%s  ",     get_localized_string(g_renderstate->language, "user.interface.stats.potion.off")
                );

    wattroff(g_renderstate->wnd, A_BOLD);


/******************************************************
 *                                                    *
 *       Estatisticas no Retângulo da Direita         *
 *                                                    *
 *****************************************************/


    mvwprintw(g_renderstate->wnd,
              1,
              LARGURA_RETANGULO + LARGURA_LOGO + 6 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.item")),
              "%s    ",     g_gamestate->player->item->name
            );


    mvwprintw(g_renderstate->wnd,
              1 + 1,
              LARGURA_RETANGULO + LARGURA_LOGO + 6 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.item.damage")),
              "%d  ",     g_gamestate->player->item->damage
            );


    mvwprintw(g_renderstate->wnd,
              1 + 2,
              LARGURA_RETANGULO + LARGURA_LOGO + 6 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.candles")),
              "%d  ",     g_gamestate->player->current_candle
            );


    mvwprintw(g_renderstate->wnd,
              1 + 3,
              LARGURA_RETANGULO + LARGURA_LOGO + 6 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.trap")),
              "%d  ",     g_gamestate->player->trap
            );


    mvwprintw(g_renderstate->wnd,
              1 + 4,
              LARGURA_RETANGULO + LARGURA_LOGO + 6 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.molotov")),
              "%d  ",     g_gamestate->player->molotov
            );
    
    if(g_gamestate->player->hasUltimate == 0)
        mvwprintw(g_renderstate->wnd,
              1 + 5,
              LARGURA_RETANGULO + LARGURA_LOGO + 4,
              "%s  ",     get_localized_string(g_renderstate->language, "user.interface.stats.ultimate.off")
            );
    else if(g_gamestate->player->hasUltimate == 1)
        mvwprintw(g_renderstate->wnd,
              1 + 5,
              LARGURA_RETANGULO + LARGURA_LOGO + 4,
              "%s  ",     get_localized_string(g_renderstate->language, "user.interface.stats.ultimate.on")
            );

    wrefresh(g_renderstate->wnd);
    return;
}