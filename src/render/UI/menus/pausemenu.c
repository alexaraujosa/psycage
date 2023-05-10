#include "pausemenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 3
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

static unsigned short int effect = 0, botao_selecionado_principal = 0;
static char *botoes[BOTOES] = {"menu.pause.return", "menu.pause.options", "menu.pause.exit"};



void drawPauseMenu(Menu menu) {


    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


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

    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);


    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect)
            wattron(menu->wnd, A_REVERSE);

        mvwprintw(menu->wnd, yMAX/2 - ALTURA_PAUSE/2 + separador + i +1 , xMAX/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2, "%s", get_localized_string(g_renderstate->language, botoes[i]));
        
        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);

    }

}

void tick_PauseMenu() {
    return;
}


void handle_PauseMenu_keybinds(int key) {


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

                            case 0 : 
                                closeMenu(MENU_PAUSE);
                                break;
                            
                            case 1 : 
                                displayMenu(MENU_OPTIONS); 
                                break;
                            case 2 : 
                                closeMenu(MENU_PAUSE);      // Aqui devo fazer clean do PATH todo dos menus
                                displayMenu(MENU_MAIN_MENU); 
                                break;

                        }

    }
    
}