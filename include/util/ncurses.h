#ifndef __RL_UTIL_NCURSES_H
#define __RL_UTIL_NCURSES_H

#include <ncurses.h>
#include "../src/render/render.h"

// Definição dos valores relativos às dimensões da janela
#define xMAX g_renderstate->ncols
#define yMAX g_renderstate->nrows

// Definição dos valores do Logo (Utilizado em outros Menus)
#define ALTURA_LOGO 7
#define LARGURA_LOGO 66


inline void rectangle(WINDOW* win, int y1, int x1, int y2, int x2) {
    mvwhline(win, y1, x1, 0, x2-x1);
    mvwhline(win, y2, x1, 0, x2-x1);
    mvwvline(win, y1, x1, 0, y2-y1);
    mvwvline(win, y1, x2, 0, y2-y1);
    mvwaddch(win, y1, x1, ACS_ULCORNER);
    mvwaddch(win, y2, x1, ACS_LLCORNER);
    mvwaddch(win, y1, x2, ACS_URCORNER);
    mvwaddch(win, y2, x2, ACS_LRCORNER);
}

static inline void printer(WINDOW* win, int yinicio, int xinicio) {
   
    static char *logo[] = {
        " `7MMpdMAo. ,pP\"Ybd `7M'   `MF',p6\"bo   ,6\"Yb.  .P\"Ybmmm .gP\"Ya  ",
        "  MM   `Wb 8I   `\"   VA   ,V 6M'  OO  8)   MM :MI  I8  ,M'   Yb      ",
        "  MM    M8 `YMMMa.    VA ,V  8M        ,pm9MM  WmmmP\"  8M\"\"\"\"\"\"",
        "  MM   ,AP L.   I8     VVV   YM.    , 8M   MM 8M       YM.    ,       ",
        "  MMbmmd'  M9mmmP'     ,V     YMbmd'  `Moo9^Yo.YMMMMMb  `Mbmmd'       ",
        "  MM                  ,V                      6'     dP               ",
        ".JMML.             OOb\"                       Ybmmmd'                "
    };


    for (int i = 0; i < ALTURA_LOGO ; i++) {

        for (int j = 0; j < LARGURA_LOGO ; j++) {

            unsigned short int pair = 0;

            /* Dependendo do Caractere, o Par de Cores Ativo difere */

            switch(logo[i][j]) {
                case 'M' : pair = LIGHT_ORANGE_LOGO; break;
                case '"' : case '9' : pair = DARK_ORANGE_LOGO; break;
                case '.' : case ',' : case '`' : pair = LIGHTPLUS_GREY_LOGO; break;
                case 'm' : pair = YELLOW_ORANGE_LOGO; break;
                default : pair = ORANGE_LOGO;
            }

            /* Ativação do par de cores, print do caractere, desativação do par de cores, respetivamente */
            wattron(win, COLOR_PAIR(pair));

            mvwprintw(win, yinicio + i, xinicio + j, "%c", logo[i][j]);

            wattroff(win, COLOR_PAIR(pair));
        }
    }
}

inline char* tamanho_maxPalavra(int qnt_botoes, char *botoes[]) {
    char *tamanho_max = (get_localized_string(g_renderstate->language, botoes[0]));

    for (int i = 1; i < qnt_botoes; i++) {

        if (strlen(get_localized_string(g_renderstate->language, botoes[i])) > strlen(get_localized_string(g_renderstate->language, tamanho_max)))
            tamanho_max = get_localized_string(g_renderstate->language, botoes[i]);
        
    }

    return tamanho_max;
    
}


#endif