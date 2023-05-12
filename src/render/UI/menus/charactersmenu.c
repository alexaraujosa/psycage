#include "charactersmenu.h"
#include "util/ncurses.h"

#define BOTOES 3
#define CLASSES 3
#define ESPACAMENTO 2
#define TAMANHO_MAX_BOTAO 20
#define TAMANHO_MAX_CLASSE 20

static unsigned short int botao_selecionado_principal = 0, effect = 0;
static char *botoes[BOTOES] = {"menu.characters.priest", "menu.characters.detective", "menu.characters.mercenary"};




void draw_CharactersMenu(Menu menu) {
    
    // Get the width of the widest button
    char *botao_Maior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanho_Botao_Maior = strlen(botao_Maior);

    // Class Choice Message
    static char *classe_Mensagem = NULL;
    classe_Mensagem = (g_renderstate->language == en_US) ? "Choose your Character Class:" : "Escolha a Classe do seu Jogador:";

    // Print the logo
    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);

    // Print of the Class Choice Message Underlined
    wattron(menu->wnd, A_UNDERLINE);
    mvwprintw(menu->wnd, yMAX/3, xMAX/2 - strlen(classe_Mensagem)/2 + 1, "%s", classe_Mensagem);
    wattroff(menu->wnd, A_UNDERLINE);

    // Create the rectangle around the classes
    rectangle(menu->wnd, 
              yMAX/3 + ESPACAMENTO           , xMAX/2 - tamanho_Botao_Maior/2 -1,
              yMAX/3 + ESPACAMENTO + BOTOES*2, xMAX/2 + tamanho_Botao_Maior/2 + (g_renderstate->language == en_US ? 1 : 0)
            );

    // Prints the buttons to choose the class (The selected one is highlighted)
    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, 
                  yMAX/3 + ESPACAMENTO + separador + i + 1,
                  xMAX/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2,
                  "%s", get_localized_string(g_renderstate->language, botoes[i])
                );

        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);
            
    }

    // Creates the window with the description of each class
    WINDOW *info = malloc(sizeof(WINDOW));
    info = newwin(
                    yMAX/3                             , xMAX/2, 
                    yMAX/3 + ESPACAMENTO + BOTOES*2 + 2, xMAX/4
    );

        // Creates a rectangle around the window
        box(info, 0, 0);

        // Display the description of each class
        wattron(info, A_BOLD | A_UNDERLINE);

        switch(botao_selecionado_principal) {
            
            case 0 : {
                mvwprintw(info, 1, 2, "%s", get_localized_string(g_renderstate->language, botoes[0]));
                wattroff(info, A_BOLD | A_UNDERLINE);

                // Print da descrição da class
                break;
            }
            
            case 1 : {
                mvwprintw(info, 1, 2, "%s", get_localized_string(g_renderstate->language, botoes[1]));
                wattroff(info, A_BOLD | A_UNDERLINE);

                // Print da descrição da class
                break;
            }
            
            case 2 : {
                mvwprintw(info, 1, 2, "%s", get_localized_string(g_renderstate->language, botoes[2]));
                wattroff(info, A_BOLD | A_UNDERLINE);

                // Print da descrição da class
                break;
            }

    }

    // Refreshes both windows
    wrefresh(menu->wnd);
    wrefresh(info);

}






void tick_CharactersMenu() {
    return;
}


void handle_CharactersMenu_keybinds(int key) {

    if(botao_selecionado_principal == 0 && key == KEY_UP) {

        botao_selecionado_principal = BOTOES - 1;
        effect = BOTOES - 1;

        return;
    }

    if(botao_selecionado_principal == BOTOES-1 && key == KEY_DOWN) {

        botao_selecionado_principal = 0;
        effect = 0;

        return;
    }
    
    switch(key) {

        case KEY_UP :

            botao_selecionado_principal--;
            effect--;
            break;


        case KEY_DOWN :

            botao_selecionado_principal++;
            effect++;
            break;


        case 10 : case 13 : switch(botao_selecionado_principal) {

                            case Priest : {
                                g_gamestate->player->entity->basedamage = 3;
                                g_gamestate->player->entity->class = Priest;
                                //startgame
                                break;            
                            }

                            case Detective : {
                                g_gamestate->player->entity->basedamage = 3;
                                g_gamestate->player->entity->class = Detective;
                                //startgame
                                break;             
                            }

                            case Mercenary : {
                                g_gamestate->player->entity->basedamage = 3;
                                g_gamestate->player->entity->class = Mercenary;
                                //startgame
                                break;             
                            }

                        }

    }



}