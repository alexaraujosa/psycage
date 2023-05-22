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
#define ESTATISTICAS 6

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

// void drawMenu(Menu menu) {
//     switch (menu->id) {
//         case MENU_NONE: break;
//         case MENU_MAIN_MENU: {
//             WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
//             PANEL* panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_MainMenu(menu);
//             break;
//         }
//         case MENU_DIALOG: {
//             WINDOW* win = newwin( 
//                 g_ui_size[0],
//                 g_ui_size[1],
//                 (g_renderstate->nrows / 2) - (g_ui_size[0] / 2), 
//                 (g_renderstate->ncols / 2) - (g_ui_size[1] / 2)
//             );
//             PANEL* panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_Dialog(menu);
//             break;
//         }
//         case MENU_OPTIONS: {
//             WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_OptionsMenu(menu);
//             break;
//         }
//         case MENU_PAUSE: {
//             WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_PauseMenu(menu);
//             break;
//         }
//         case MENU_DEAD: {
//             WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_DeadMenu(menu);
//             break; 
//         }
//         case MENU_CHARACTERS: {
//             WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_CharactersMenu(menu);
//             break;
//         }
//         case MENU_CHARACTERS_INFO: {
//             WINDOW* win = newwin(yMAX/3, xMAX/2, ALTURA_CHARACTERS_INFO, LARGURA_CHARACTERS_INFO);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_CharactersInfo(menu);
//             break;
//         }
//         case MENU_SAVE: {
//             WINDOW* win = newwin(g_renderstate->nrows, g_renderstate->ncols, 0, 0);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_SaveMenu(menu);
//             break;
//         }
//         case MENU_SAVE_SLOT: {
//             WINDOW* win = newwin(g_renderstate->nrows/3, g_renderstate->ncols/4, g_renderstate->nrows/2, 3*g_renderstate->ncols/8);
//             PANEL * panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_SaveInfo(menu);
//             break;
//         }
//         case MENU_CONSOLE: {
//             WINDOW* win = newwin(g_renderstate->nrows - 6, g_renderstate->ncols - 6, 3, 3);
//             // WINDOW* win = newwin(10, 10, 3, 3);
//             PANEL* panel = new_panel(win);
//             menu->wnd = win;
//             menu->panel = panel;

//             draw_ConsoleMenu(menu);
//             break;
//         }
//         default:
//             break;
//     }
// }

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
        case MENU_PAUSE: {
            // tick_PauseMenu();
            break;
        }
        case MENU_DEAD: {
            // tick_DeadMenu();
            break;
        }
        case MENU_CHARACTERS: {
            // tick_CharactersMenu();
            break;
        }
        case MENU_CHARACTERS_INFO: {
            // tick_CharactersMenu();
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


void drawGameInterface() {

    static char *stats[ESTATISTICAS+2] = {"user.interface.stats.class", "user.interface.stats.level","user.interface.stats.kills", "user.interface.stats.armor", "user.interface.stats.hp", "user.interface.stats.xp", "user.interface.stats.monsters", "user.interface.stats.item"};

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

    //Print da quantidade de Monstros Vivos no canto inferior esquerdo do retângulo da esquerda
    mvwprintw(g_renderstate->wnd,
              ALTURA_LOGO - 1,
              2,
              "%s",     get_localized_string(g_renderstate->language, stats[ESTATISTICAS])   
            );

    //Print do Item Atual no meio do retângulo da esquerda
    mvwprintw(g_renderstate->wnd,
              ALTURA_LOGO - 1,
              LARGURA_RETANGULO*0.4,
              "%s  ",     get_localized_string(g_renderstate->language, stats[ESTATISTICAS+1])   
            );

    wattroff(g_renderstate->wnd, A_BOLD);


    //Print dos valores de cada estatística


/*      Lado esquerdo do Retângulo da esquerda   */


    //Print da Classe
    mvwprintw(g_renderstate->wnd, 
              ALTURA_LOGO/3,    
              LARGURA_RETANGULO/6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.class")),
              "%s   " , getClassInterface(g_gamestate->player->class)
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
    if(g_gamestate->player->cheats->godmode == 0) {

        mvwprintw(g_renderstate->wnd,
                ALTURA_LOGO/3 + 1, 
                LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.hp")),
                "%d/%d     ", g_gamestate->player->entity->health, g_gamestate->player->entity->maxHealth
                );

    } else {

        mvwprintw(g_renderstate->wnd,
                ALTURA_LOGO/3 + 1, 
                LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.hp")),
                "%s", get_localized_string(g_renderstate->language, "user.interface.stats.god")
                );

    }

    //Print do XP
    mvwprintw(g_renderstate->wnd, 
            ALTURA_LOGO/3 + 2,    
            LARGURA_RETANGULO*0.6 + 2 + strlen(get_localized_string(g_renderstate->language, "user.interface.stats.xp")),
            "%d", g_gamestate->player->xp
        );

    //Print da quantidade de Monstros Vivos
    mvwprintw(g_renderstate->wnd,
              ALTURA_LOGO - 1,
              3 + strlen(get_localized_string(g_renderstate->language, stats[ESTATISTICAS])),
              "%d",     g_gamestate->mob_count   
            );

    //Print do Item atual
    mvwprintw(g_renderstate->wnd,
              ALTURA_LOGO - 1,
              LARGURA_RETANGULO*0.4 + 1 + strlen(get_localized_string(g_renderstate->language, stats[ESTATISTICAS+1])),
              "%s  ",     g_gamestate->player->item->name
            );

// CONSOLA

    //Print das mensagens da Consola
    for(int i = 0 ; i < MAX_MESSAGES ; i++) 
        mvwprintw(g_renderstate->wnd, i + 1, g_renderstate->ncols/2 + LARGURA_LOGO*0.55, "aa", g_gamestate->messages[i]);

    return;
}

void addConsoleMessage(int value, char* key) {

    /*
    O que vai aparecer:
    (XP) - Quando um player ganha XP(e.g. mata um mob) vai aparecer que recebeu X xp
    (HP) - Quando um player ataca um MOB, mostra quanto HP tirou e, quando um player é atacado, quanto de HP perdeu
    (ITEM) - Quando um player abre um Chest vai aparecer que trocou de ITEM (e.g. You got a Sword) [o Sword seria o ITEM]
    */

    for(int i = MAX_MESSAGES - 1 ; i > 0 ; i--)
        strcpy(g_gamestate->messages[i], g_gamestate->messages[i-1]);

    char mensagem[MAX_MESSAGES_LENGTH];
    strcpy(mensagem, get_localized_string(g_renderstate->language, key));

    sprintf(mensagem, mensagem, value);
    strcpy(g_gamestate->messages[0], mensagem);

}