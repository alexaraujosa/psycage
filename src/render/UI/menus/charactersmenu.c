#include "charactersmenu.h"
#include "util/ncurses.h"

#define CLASSES 3
#define TAMANHO_MAX_BOTAO 20
#define TAMANHO_MAX_CLASSE 20
#define PRIEST_INFOS 6
#define DETECTIVE_INFOS 6
#define MERCENARY_INFOS 6

static unsigned short int botao_selecionado_principal = 0;
static char *botoes[BOTOES_CHARACTERS] = {"menu.characters.priest", "menu.characters.detective", "menu.characters.mercenary"};




void draw_CharactersMenu(Menu menu) {
    
    // Get the width of the widest button
    char *botao_Maior = tamanho_maxPalavra(BOTOES_CHARACTERS, botoes);
    unsigned short int tamanho_Botao_Maior = strlen(botao_Maior);

    // Print the logo
    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);

    // Print of the Class Choice Message
    mvwprintw(menu->wnd, yMAX/3, xMAX/2 - strlen(get_localized_string(g_renderstate->language, "menu.characters.choose"))/2 + 1, "%s", get_localized_string(g_renderstate->language, "menu.characters.choose"));

    // Create the rectangle around the classes
    rectangle(menu->wnd, 
              yMAX/3 + ESPACAMENTO_CHARACTERS           , xMAX/2 - tamanho_Botao_Maior/2 -1,
              yMAX/3 + ESPACAMENTO_CHARACTERS + BOTOES_CHARACTERS*2, xMAX/2 + tamanho_Botao_Maior/2 + (g_renderstate->language == en_US ? 1 : 0)
            );

    // Prints the buttons to choose the class (The selected one is highlighted)
    for(int i = 0, separador = 0 ; i < BOTOES_CHARACTERS ; i++, separador += 1) {

        if(i == botao_selecionado_principal) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, 
                  yMAX/3 + ESPACAMENTO_CHARACTERS + separador + i + 1,
                  xMAX/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2,
                  "%s", get_localized_string(g_renderstate->language, botoes[i])
                );

        if(i == botao_selecionado_principal)
            wattroff(menu->wnd, A_REVERSE);
            
    }
    displayMenu(MENU_CHARACTERS_INFO);

}

void draw_CharactersInfo(Menu menu){

    static char *info[] = {"menu.characters.info.priest.one", "menu.characters.info.priest.two", "menu.characters.info.priest.three", "menu.characters.info.priest.four", "menu.characters.info.priest.five", "menu.characters.info.priest.six",
                      "menu.characters.info.detective.one", "menu.characters.info.detective.two", "menu.characters.info.detective.three", "menu.characters.info.detective.four", "menu.characters.info.detective.five", "menu.characters.info.detective.six", 
                      "menu.characters.info.mercenary.one", "menu.characters.info.mercenary.two", "menu.characters.info.mercenary.three", "menu.characters.info.mercenary.four", "menu.characters.info.mercenary.five", "menu.characters.info.mercenary.six", 
                    };

    box(menu->wnd, 0 , 0);

        // Display the description of each class
        wattron(menu->wnd, A_BOLD | A_UNDERLINE);

        switch(botao_selecionado_principal) {
            
            case 0 : {
                mvwprintw(menu->wnd, 1, 2, "%s", get_localized_string(g_renderstate->language, botoes[0]));
                wattroff(menu->wnd, A_BOLD | A_UNDERLINE);

                for(int i = 0 ; i < PRIEST_INFOS ; i++)
                    mvwprintw(menu->wnd, 4 + i, 2, "%s", get_localized_string(g_renderstate->language, info[i]));
                break;
            }
            
            case 1 : {
                mvwprintw(menu->wnd, 1, 2, "%s", get_localized_string(g_renderstate->language, botoes[1]));
                wattroff(menu->wnd, A_BOLD | A_UNDERLINE);

                for(int i = PRIEST_INFOS, j = 0 ; i < PRIEST_INFOS+DETECTIVE_INFOS ; i++, j++)
                    mvwprintw(menu->wnd, 4 + j, 2, "%s", get_localized_string(g_renderstate->language, info[i]));
                break;
            }
            
            case 2 : {
                mvwprintw(menu->wnd, 1, 2, "%s", get_localized_string(g_renderstate->language, botoes[2]));
                wattroff(menu->wnd, A_BOLD | A_UNDERLINE);

                for(int i = PRIEST_INFOS + DETECTIVE_INFOS, j = 0 ; i < PRIEST_INFOS+DETECTIVE_INFOS+MERCENARY_INFOS ; i++, j++)
                    mvwprintw(menu->wnd, 4 + j, 2, "%s", get_localized_string(g_renderstate->language, info[i]));
                break;
            }

    }

}




void tick_CharactersMenu() {
    return;
}


void handle_CharactersMenu_keybinds(int key) {

    if(botao_selecionado_principal == 0 && key == KEY_UP) {

        botao_selecionado_principal = BOTOES_CHARACTERS - 1;

        return;
    }

    if(botao_selecionado_principal == BOTOES_CHARACTERS-1 && key == KEY_DOWN) {

        botao_selecionado_principal = 0;

        return;
    }
    
    switch(key) {

        case KEY_UP :

            botao_selecionado_principal--;
            break;


        case KEY_DOWN :

            botao_selecionado_principal++;
            break;


        case 10 : case 13 : 
                switch(botao_selecionado_principal) {

                            case Priest : {
                                g_gamestate->player->entity->basedamage = 12;
                                g_gamestate->player->class = Priest;
                                g_gamestate->player->radius = 2;
                                g_gamestate->player->entity->health = 8;
                                g_gamestate->player->entity->maxHealth = 8;
                                break;            
                            }

                            case Detective : {
                                g_gamestate->player->entity->basedamage = 4;
                                g_gamestate->player->class = Detective;
                                g_gamestate->player->radius = 8;
                                g_gamestate->player->entity->health = 16;
                                g_gamestate->player->entity->maxHealth = 16;
                                break;             
                            }

                            case Mercenary : {
                                g_gamestate->player->entity->basedamage = 8;
                                g_gamestate->player->class = Mercenary;
                                g_gamestate->player->radius = 5;
                                g_gamestate->player->entity->health = 12;
                                g_gamestate->player->entity->maxHealth = 12;
                                break;             
                            }

                        }
                start_game();

    }



}


void cleanup_characters_menu() {
    botao_selecionado_principal = 0;
}
