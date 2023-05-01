#include "pausemenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 2

static char *botoes[] = {"   Return   ", "    Exit    "};   // resume, save, options, exit
static int botao_selecionado = 0, effect = 0;

void drawPauseMenu(Menu menu) {
    
    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    int tamanhoBotaoMaior = strlen(botaoMaior);


    static char *pause[] = {
        "  8888888b.      d8888 888     888  .d8888b.  8888888888    ",
        "  888   Y88b    d88888 888     888 d88P  Y88b 888           ",
        "  888    888   d88P888 888     888 Y88b.      888           ",
        "  888   d88P  d88P 888 888     888  \"Y888b.   8888888   ",  
        "  8888888P\"  d88P  888 888     888     \"Y88b. 888         ",   
        "  888       d88P   888 888     888       \"888 888          ",  
        "  888      d8888888888 Y88b. .d88P Y88b  d88P 888           ", 
        "  888     d88P     888  \"Y88888P\"   \"Y8888P\"  8888888888" 
                                                       
    };


    /* Obter altura e largura da ASCII - Pause */

    int altura_pause = sizeof(pause) / sizeof(pause[0]);
    int largura_pause = strlen(pause[4]);


    /* Obter onde vai ser colocada a ASCII - Pause em xOy */

    int x_pause = g_renderstate->ncols/2 - largura_pause/2;
    int y_pause = g_renderstate->nrows   - altura_pause*4/3;


    /* Criar o retângulo que liga as duas ASCII (Logo e Pause). O -1 serve para alinhar o traço inferior do retângulo com o traço do meio do E */

    rectangle(menu->wnd,
              sizeof(logo)/sizeof(logo[0])  , x_pause - 8, 
              y_pause + altura_pause/2 -1   , x_pause + largura_pause + 7
    );


    /* Printer da ASCII - Pause em Negrito, com efeito Itálico */
    
    wattron(menu->wnd, A_BOLD);

    for(int i = 0 ; i < altura_pause ; i++)
        for(int j = 0 ; j < largura_pause ; j++)
            mvwprintw(menu->wnd, y_pause + i, x_pause + j + i, "%c", pause[i][j]);

    wattroff(menu->wnd, A_BOLD);


    /* Cria o retângulo à volta dos botões */

    rectangle(
              menu->wnd, 
              g_renderstate->nrows/2 - altura_pause/2           , g_renderstate->ncols/2 - tamanhoBotaoMaior/2 - 1, 
              g_renderstate->nrows/2 - altura_pause/2 + BOTOES*2, g_renderstate->ncols/2 + tamanhoBotaoMaior/2 
             );


    /* Print do logo */
    
    printer(menu->wnd);


    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect)
            wattron(menu->wnd, A_REVERSE);

        mvwprintw(menu->wnd, g_renderstate->nrows/2 - altura_pause/2 + separador + i +1 , g_renderstate->ncols/2 - strlen(botoes[i])/2, "%s", botoes[i]);
        
        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);

    }




}

void tick_PauseMenu() {
    return;
}


void handle_PauseMenu_keybinds(int key) {

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

                            case 0 : break; //return
                            case 1 : closeMenu(MENU_PAUSE); displayMenu(MENU_MAIN_MENU); break; //exit

                        }

    }
    
}