#include "optionsmenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 2

static unsigned short int effect = 0;

void drawOptionsMenu(Menu menu) {
    
    static char *botoes[BOTOES] = {"   Return   ", "  Language  ", "Tutorial ", " Options ", "Quit Game"}; 

    if(g_renderstate->language == pt_PT) {
        
        for(int i = 0 ; i < BOTOES ; i++)
            botoes[i] = (char *) malloc(strlen(botoes[i] + 1));

        strcpy(botoes[0], "   Voltar   ");
        strcpy(botoes[1], "  Linguagem ");
    
    }


    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


#define LARGURA_OPTIONS 76
#define ALTURA_OPTIONS 8


    static char *options[ALTURA_OPTIONS] = {
        " .d88888b.  8888888b. 88888888888 8888888 .d88888b.  888b    888  .d8888b.      ",
        "888     888 888    888    888       888  888     888 88888b  888 Y88b.          ",
        "d88     88b 888   Y88b    888       888  d88     88b 8888b   888 d88P           ",
        "888     888 888   d88P    888       888  888     888 888Y88b 888  \"Y888b.      ",
        "888     888 8888888P\"     888       888  888     888 888 Y88b888     \"Y88b.",
        "888     888 888           888       888  888     888 888  Y88888       \"888    ",
        "Y88b. .d88P 888           888       888  Y88b. .d88P 888   Y8888 Y88b  d88P     ",
        " \"Y88888P\"  888           888     8888888 \"Y88888P\"  888    Y888  \"Y8888P\""
    };


    /* Obter onde vai ser colocada a ASCII - Options em xOy */

    unsigned short int x_options = xMAX/2 - LARGURA_OPTIONS/2;
    unsigned short int y_options = yMAX   - ALTURA_OPTIONS*4/3;


    /* Criar o retângulo que liga as duas ASCII (Logo e Options) */

    rectangle(menu->wnd, 
              ALTURA_LOGO, x_options - 6,
              y_options + ALTURA_OPTIONS/2, x_options + LARGURA_OPTIONS + 5
    );
    

    /* Printer da ASCII - Options em Negrito */

    wattron(menu->wnd, A_BOLD);

    for(int i = 0 ; i < ALTURA_OPTIONS ; i++)
        for(int j = 0 ; j < LARGURA_OPTIONS ; j++)
            mvwprintw(menu->wnd, y_options + i, x_options + j, "%c", options[i][j]);

    wattroff(menu->wnd, A_BOLD);


    /* Cria o retângulo à volta dos botões */
    
    rectangle(menu->wnd, 
              g_renderstate->nrows/2 - ALTURA_OPTIONS/2           , g_renderstate->ncols/2 - tamanhoBotaoMaior/2 - 1, 
              g_renderstate->nrows/2 - ALTURA_OPTIONS/2 + BOTOES*2, g_renderstate->ncols/2 + tamanhoBotaoMaior/2 
             );


    /* Print do logo */
    
    printer(menu->wnd);


    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, g_renderstate->nrows/2 - ALTURA_OPTIONS/2 + separador + i +1 , g_renderstate->ncols/2 - strlen(botoes[i])/2, "%s", botoes[i]);
        
        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);

    }

}

void tick_OptionsMenu() {
    return;
}


void handle_OptionsMenu_keybinds(int key) {

    static unsigned short int botao_selecionado = 0;

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

                            case 0 :{
                            //     //if(isInGame) {
                            //         closeMenu(MENU_OPTIONS);
                            //         break;
                            //         //display game
                            //     //else if(isInMainMenu) {  
                            //         closeMenu(MENU_OPTIONS);
                            //         displayMenu(MENU_MAIN_MENU);
                            //         break;
                            //     }
                            //     //else if(isDead) {
                            //         closeMenu(MENU_OPTIONS);
                            //         display(MENU_DEAD);
                            //         break;
                            //     }
                            //     }
                            // 
                            }
                            case 1 :{
                                g_renderstate->language = (g_renderstate->language == en_US) ? pt_PT : en_US;
                                break;
                            }
                        }

    }

}