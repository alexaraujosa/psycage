#include "mainmenu.h"
#include "util/ncurses.h"
#include "../../render.h"
#include <stdlib.h>

#define BOTOES 5
#define TAMANHO_MAX_BOTAO 20

static unsigned short int botao_selecionado = 0, effect = 0;

void drawMainMenu(Menu menu) {
    
    char **botoes = malloc(BOTOES * sizeof(char *));

    for (int i = 0; i < BOTOES; i++)
        botoes[i] = malloc(TAMANHO_MAX_BOTAO * sizeof(char));


    strcpy(botoes[0], "New  Game");
    strcpy(botoes[1], "Load Game");
    strcpy(botoes[2], "Tutorial ");
    strcpy(botoes[3], " Options ");
    strcpy(botoes[4], "Quit Game");


    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


    static char *cage[] = {
        "                        .#     &                       ",
        "                       #         #                     ",
        "                       /         #                     ",
        "                       .(  ##/  #                      ",
        "                          (&##.                        ",
        "                           *&,                         ",
        "                 .&#################&.                 ",
        "            #.# #  #  #    #    (  #  & #.#            ",
        "         & # .   #   /     #     .   #  ,  # #         ",
        "       # #  #   &          #     *    #   #  & #       ",
        "     *     *   #    #      #      ,    #   #  *        ",
        "    *  .  #    .    #      #      #    .    #  (  .    ",
        "    , #       ,     /      #      #     .   .   ( (    ",
        "      #  .    (     .      &      (     /       &      ",
        "      #  .    (     .      #      (     /       #      ",
        "      #  .    (     .             (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "      #  .    (     .             (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "      #  .    (     .             (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "      #  .    (     .             (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "      #  .    (     .             (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "      &  .    (     ,             (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "      #  .    (     .      #      (     /       #      ",
        "    &##############################################&   ",
        "  /##################################################/ ",
        "  &##################################################& "
    };


    /* Obter altura e largura da ASCII - Cage */

    unsigned short int altura_cage = sizeof(cage) / sizeof(cage[0]);
    unsigned short int largura_cage = strlen(cage[0]);


    /* Obter onde vai ser colocada a ASCII - Cage em xOy */

    unsigned short int x_cage = g_renderstate->ncols/2 - largura_cage/2;
    unsigned short int y_cage = g_renderstate->nrows/2 - altura_cage/3;


    /* Printer da ASCII - Cage */

    for(int i = 0 ; i < altura_cage ; i++) {

        for(int j = 0 ; j < largura_cage ; j++) {

            unsigned short int pair = 0;

            switch(cage[i][j]) {
                case '&' : case '(' : case '/' : case '*' : pair = LIGHTPLUS_GREY_LOGO; break;
                default : pair = DARKPLUS_GREY_LOGO;
            }

            wattron(menu->wnd, COLOR_PAIR(pair));

            mvwprintw(menu->wnd, y_cage + i, x_cage + j, "%c", cage[i][j]);

            wattroff(menu->wnd, COLOR_PAIR(pair));
        }

    }


    /* Cria o retângulo à volta dos botões */
    
    rectangle(menu->wnd, 
              g_renderstate->nrows/2 + 5           , g_renderstate->ncols/2 - tamanhoBotaoMaior/2 - 1,
              g_renderstate->nrows/2 + 5 + BOTOES*2, g_renderstate->ncols/2 + tamanhoBotaoMaior/2 + 1
    );


    /* Print do logo */

    printer(menu->wnd);

    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor e +5 devido ao circulo no topo da ASCII ART */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, g_renderstate->nrows/2 + separador + i + 5 +1 , g_renderstate->ncols/2 - strlen(botoes[i])/2, "%s", botoes[i]);

        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);
            
    }


    for (int i = 0; i < BOTOES; i++)
        free(botoes[i]);

    free(botoes);

}

void tick_MainMenu() {
    return;
}

void handle_MainMenu_keybinds(int key) {

    if(botao_selecionado == 0 && key == KEY_UP) {

        botao_selecionado = BOTOES - 1;
        effect = BOTOES - 1;

        return;
    }

    if(botao_selecionado == BOTOES-1 && key == KEY_DOWN) {

        botao_selecionado = 0;
        effect = 0;

        return;
    }
    
    switch(key) {

        case KEY_UP :
            botao_selecionado--;
            effect--;
            break;


        case KEY_DOWN :
            botao_selecionado++;
            effect++;
            break;


        case 10 : case 13 : switch(botao_selecionado) {

                            case 0 : break; //new game
                            case 1 : break; //load game
                            case 2 : break; //tutorial
                            case 3 : displayMenu(MENU_OPTIONS); break;
                            case 4 : endwin(); exit(0);

                        }

    }
    
}