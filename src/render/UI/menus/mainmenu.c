#include "mainmenu.h"
#define BOTOES 5
static char *botoes[] = {"NEW GAME", "LOAD GAME", "TUTORIAL", "OPTIONS", "QUIT GAME"};
static int botao_selecionado = 0, effect = 0;

void drawMainMenu(Menu menu) {
    
    /* Definição do Par 1 - Laranja */
    init_color(8, 830, 470, 0);
    init_pair(1, 8, 0);

    /* Definição do Par 2 - Laranja Menos Escuro */
    init_color(9, 800, 500, 0);
    init_pair(2, 9, 0);

    /* Definição do Par 3 - Laranja Mais Escuro */
    init_color(10, 760, 390, 0);
    init_pair(3, 10, 0);

    /* Definição do Par 4 - Cinzento Claro */
    init_color(11, 768, 768, 768);
    init_pair(4, 11, 0);

    /* Definição do Par 5 - Laranja Amarelado */
    init_color(12, 870, 580, 0);
    init_pair(5, 12, 0);
    

    static char *logo[] = {
        " `7MMpdMAo. ,pP\"Ybd `7M'   `MF',p6\"bo   ,6\"Yb.  .P\"Ybmmm .gP\"Ya  ",
        "  MM   `Wb 8I   `\"   VA   ,V 6M'  OO  8)   MM :MI  I8  ,M'   Yb ",
        "  MM    M8 `YMMMa.    VA ,V  8M        ,pm9MM  WmmmP\"  8M\"\"\"\"\"\" ",
        "  MM   ,AP L.   I8     VVV   YM.    , 8M   MM 8M       YM.    , ",
        "  MMbmmd'  M9mmmP'     ,V     YMbmd'  `Moo9^Yo.YMMMMMb  `Mbmmd' ",
        "  MM                  ,V                      6'     dP         ",
        ".JMML.             OOb\"                       Ybmmmd'           "
    };

    /* Obter a altura e a largura da ASCII ART */
    int altura_logo = sizeof(logo) / sizeof(logo[0]);
    int largura_logo = strlen(logo[0]);

    /* Obter as coordenadas onde vai ser posicionada a ASCII ART */
    int x_logo = g_renderstate->ncols/2 - largura_logo/2;
    int y_logo = g_renderstate->nrows/5 - altura_logo/2;

    /* Obter o Offset que será usado para corrigir o posicionamente da primeira linha */
    int offset_2linha_logo = strlen(logo[1]) - strlen(logo[0]);

    /* Print do logo com o Offset corrigido */
    for (int i = 0; i < altura_logo; i++) {

        for (int j = 0; j < largura_logo; j++) {
            int pair = 0;
            /* Dependendo do Caractere, o Par de Cores Ativo difere */
            switch(logo[i][j]) {
                case 'M' : pair = 2; break;
                case '"' : case '9' : pair = 3; break;
                case '.' : case ',' : case '`' : pair = 4; break;
                case 'm' : pair = 5; break;
                default : pair = 1;
            }
            /* Ativação do par de cores, print do caractere, desativação do par de cores */
            wattron(menu->wnd, COLOR_PAIR(pair));
            mvwprintw(menu->wnd, y_logo + i, x_logo + j + (i == 1 ? offset_2linha_logo : 0), "%c", logo[i][j]);
            wattroff(menu->wnd, COLOR_PAIR(pair));
            refresh();
        }
    }

    for(int i = 0, separador = 0 ; i < (int) (sizeof(botoes)/sizeof(char *)) ; i++, separador += 1) {

        if(i == effect) {
            wattron(menu->wnd, A_REVERSE);
        }

        mvwprintw(menu->wnd, g_renderstate->nrows*2/5+i+separador, 2* g_renderstate->ncols/3, "%s", botoes[i]);
        
        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);
    }

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

    if(botao_selecionado == 4 && key == KEY_DOWN) {
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
                            case 3 : break; //options menu
                            case 4 : endwin(); exit(0);
                        }

    }
}