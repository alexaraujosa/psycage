#include "ui.h"
#include "util/ncurses.h"
#include "common.h"
#include "menus/mainmenu.h"
#include "menus/optionsmenu.h"
#include "menus/pausemenu.h"
#include "menus/deadmenu.h"
#include "menus/dialog.h"
#include "menus/charactersmenu.h"
#include "menus/savemenu.h"

#define LARGURA_RETANGULO 52
#define ESTATISTICAS 6

int g_ui_size[2] = { 0 };

void drawMenu(Menu menu) {
    switch (menu->id) {
        case MENU_NONE: break;
        case MENU_MAIN_MENU: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL* panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_MainMenu(menu);
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

            drawDialog(menu);
            break;
        }
        case MENU_OPTIONS: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_OptionsMenu(menu);
            break;
        }
        case MENU_PAUSE: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_PauseMenu(menu);
            break;
        }
        case MENU_DEAD: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_DeadMenu(menu);
            break; 
        }
        case MENU_CHARACTERS: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_CharactersMenu(menu);
            break;
        }
        case MENU_SAVE: {
            WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_SaveMenu(menu);
            break;
        }
        case MENU_SAVE_SLOT: {
            WINDOW* win = newwin(g_renderstate->nrows/3, g_renderstate->ncols/4, g_renderstate->nrows/2, 3*g_renderstate->ncols/8);
            PANEL * panel = new_panel(win);
            menu->wnd = win;
            menu->panel = panel;

            draw_SaveInfo(menu);
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
        case MENU_SAVE: {
            tick_SaveMenu();
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
        case MENU_SAVE: {
            handle_SaveMenu_keybinds(key);
            break;
        }
        case MENU_SAVE_SLOT: {
            handle_SaveMenu_keybinds(key);
            break;
        }
        default:
            break;
    }
}

void drawGameInterface() {

    static char *stats[ESTATISTICAS] = {"user.interface.stats.class", "user.interface.stats.level","user.interface.stats.kills", "user.interface.stats.armor", "user.interface.stats.hp", "user.interface.stats.xp"};

    //Desenha o retângulo à esquerda do Logo
    rectangle(g_renderstate->wnd, 0, 0, ALTURA_LOGO, g_renderstate->ncols/2 - LARGURA_LOGO/2 -1);

    //Desenha o retângulo à direita do Logo
    rectangle(g_renderstate->wnd, 0, g_renderstate->ncols/2 + LARGURA_LOGO/2 +1, ALTURA_LOGO, g_renderstate->ncols);

    //Desenha o traço por baixo do Logo
    rectangle(g_renderstate->wnd, ALTURA_LOGO, 0, g_renderstate->nrows-1, g_renderstate->ncols-1);

    //Retângulo à volta da janela toda
    rectangle(g_renderstate->wnd, 0, 0, g_renderstate->nrows-1, g_renderstate->ncols-1);

    //Print do Logo
    printer(g_renderstate->wnd, 0, g_renderstate->ncols/2 - LARGURA_LOGO/2+1);

    //Print do nome de cada estatística em Negrito

    wattron(g_renderstate->wnd, A_BOLD);


    //Print das estatísticas no lado esquerdo do retângulo da esquerda
    for(int i = 0 ; i < ESTATISTICAS/2 ; i++)
        mvwprintw(
                  g_renderstate->wnd,   
                  ALTURA_LOGO/3 + i,
                  LARGURA_RETANGULO/6,
                  "%s",    get_localized_string(g_renderstate->language, stats[i])
                );
    

    //Print das estatísticas no lado direito do retângulo da esquerda
    for(int i = ESTATISTICAS/2, j = 0 ; i < ESTATISTICAS ; i++, j++)
        mvwprintw(g_renderstate->wnd,
                  ALTURA_LOGO/3 + j,
                  LARGURA_RETANGULO*0.6,
                  "%s",    get_localized_string(g_renderstate->language, stats[i])
                );


    wattroff(g_renderstate->wnd, A_BOLD);


    //Print dos valores de cada estatística


/*      Lado esquerdo do Retângulo da esquerda   */


    //Print da Classe
    mvwprintw(g_renderstate->wnd, 
              ALTURA_LOGO/3,    
              LARGURA_RETANGULO/6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.class")),
              "%s" , getClassInterface(g_gamestate->player->class)
            );

    //Print do Level
    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 1,    
            LARGURA_RETANGULO/6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.level")),
            "%d", g_gamestate->player->level
        );

    //Print das Kills
    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 2,    
            LARGURA_RETANGULO/6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.kills")),
            "%d", g_gamestate->player->kills
        );

/*      Lado direito do Retângulo da esquerda   */


    //Print da Armor
    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3,    
            LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.armor")),
            "%d", g_gamestate->player->entity->armor
        );

    //Print do Health e Max Health
    mvwprintw(g_renderstate->wnd,
              ALTURA_LOGO/3 + 1, 
              LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.hp")),
              "%d/%d", g_gamestate->player->entity->health, g_gamestate->player->entity->maxHealth
            );

    //Print do XP
    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 2,    
            LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.xp")),
            "%d", g_gamestate->player->xp
        );

    mvwprintw(g_renderstate->wnd, ALTURA_LOGO/3, LARGURA_RETANGULO+LARGURA_LOGO + 4, "Priest got hitted by Andrew Tate and lost 5 HP.");
    mvwprintw(g_renderstate->wnd, ALTURA_LOGO/3+1, LARGURA_RETANGULO+LARGURA_LOGO + 4, "Mercenary got hitted by Andrew Tate and lost 5 HP.");
    mvwprintw(g_renderstate->wnd, ALTURA_LOGO/3+2, LARGURA_RETANGULO+LARGURA_LOGO + 4, "Detective picked a knife and earned a dick.");
    return;
}