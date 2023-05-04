#include "pausemenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 3

static unsigned short int effect = 0;

void drawPauseMenu(Menu menu) {

    static char *botoes[BOTOES] = {"   Return   ", "   Options  ", "    Exit    "}; 

    if(g_renderstate->language == pt_PT) {
        
        for(int i = 0 ; i < BOTOES ; i++)
            botoes[i] = (char *) malloc(strlen(botoes[i] + 1));

        strcpy(botoes[0], "   Voltar   ");
        strcpy(botoes[1], "   Opcoes   ");  // Falta o save
        strcpy(botoes[2], "    Sair    ");
        
    }


    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


#define LARGURA_PAUSE 59
#define ALTURA_PAUSE 8


    static char *pause[ALTURA_PAUSE] = {
        "  8888888b.      d8888 888     888  .d8888b.  8888888888    ",
        "  888   Y88b    d88888 888     888 d88P  Y88b 888           ",
        "  888    888   d88P888 888     888 Y88b.      888           ",
        "  888   d88P  d88P 888 888     888  \"Y888b.   8888888   ",  
        "  8888888P\"  d88P  888 888     888     \"Y88b. 888         ",   
        "  888       d88P   888 888     888       \"888 888          ",  
        "  888      d8888888888 Y88b. .d88P Y88b  d88P 888           ", 
        "  888     d88P     888  \"Y88888P\"   \"Y8888P\"  8888888888" 
                                                       
    };


    /* Obter onde vai ser colocada a ASCII - Pause em xOy */

    unsigned short int x_pause = xMAX/2 - LARGURA_PAUSE/2;
    unsigned short int y_pause = yMAX   - ALTURA_PAUSE*4/3;


    /* Criar o retângulo que liga as duas ASCII (Logo e Pause). O -1 serve para alinhar o traço inferior do retângulo com o traço do meio do E */

    rectangle(menu->wnd,
              ALTURA_LOGO  , x_pause - 8, 
              y_pause + ALTURA_PAUSE/2 -1   , x_pause + LARGURA_PAUSE + 7
    );


    /* Printer da ASCII - Pause em Negrito, com efeito Itálico */
    
    wattron(menu->wnd, A_BOLD);

    for(int i = 0 ; i < ALTURA_PAUSE ; i++)
        for(int j = 0 ; j < LARGURA_PAUSE ; j++)
            mvwprintw(menu->wnd, y_pause + i, x_pause + j + i, "%c", pause[i][j]);

    wattroff(menu->wnd, A_BOLD);


    /* Cria o retângulo à volta dos botões */

    rectangle(
              menu->wnd, 
              yMAX/2 - ALTURA_PAUSE/2           , xMAX/2 - tamanhoBotaoMaior/2 - 1, 
              yMAX/2 - ALTURA_PAUSE/2 + BOTOES*2, xMAX/2 + tamanhoBotaoMaior/2 
             );


    /* Print do logo */

    printer(menu->wnd);


    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect)
            wattron(menu->wnd, A_REVERSE);

        mvwprintw(menu->wnd, yMAX/2 - ALTURA_PAUSE/2 + separador + i +1 , xMAX/2 - strlen(botoes[i])/2, "%s", botoes[i]);
        
        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);

    }

}

void tick_PauseMenu() {
    return;
}


void handle_PauseMenu_keybinds(int key) {

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

                            case 0 : break; // if(IsInGame && !isDead)
                            case 1 : closeMenu(MENU_PAUSE); displayMenu(MENU_OPTIONS); break;
                            case 2 : closeMenu(MENU_PAUSE); displayMenu(MENU_MAIN_MENU); break;

                        }

    }
    
}