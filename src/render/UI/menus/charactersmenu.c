#include "charactersmenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 3
#define ESPACAMENTO 2
#define TAMANHO_MAX_BOTAO 20

static unsigned short int botao_selecionado = 0, effect = 0;

void drawCharactersMenu(Menu menu) {
    
    char **botoes = malloc(BOTOES * sizeof(char *));

    for (int i = 0; i < BOTOES; i++)
        botoes[i] = malloc(TAMANHO_MAX_BOTAO * sizeof(char));


    strcpy(botoes[0], "New  Game");
    strcpy(botoes[1], "Load Game");
    strcpy(botoes[2], "Tutorial ");     // Botoes por alterar !!!!


    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);
    char classes[] = "Choose your Character Class:";


    /* Print do logo */

    printer(menu->wnd);


    /* Print da frase para escolher a classe do personagem com effect A_UNDERLINE */

    wattron(menu->wnd, A_UNDERLINE);
    mvwprintw(menu->wnd, g_renderstate->nrows/3, g_renderstate->ncols/2 - strlen(classes)/2 + 1, "%s", classes);
    wattroff(menu->wnd, A_UNDERLINE);


    /* Cria o retângulo à volta das classes */

    rectangle(menu->wnd, 
              g_renderstate->nrows/3 + ESPACAMENTO           , g_renderstate->ncols/2 - tamanhoBotaoMaior/2 -1,
              g_renderstate->nrows/3 + ESPACAMENTO + BOTOES*2, g_renderstate->ncols/2 + tamanhoBotaoMaior/2 +1
             );


    /* Print das classes com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, g_renderstate->nrows/3 + ESPACAMENTO + separador + i +1, g_renderstate->ncols/2 - strlen(botoes[i])/2, "%s", botoes[i]);

        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);
            
    }


    /* Criação da janela abaixo das classes para colocar a devida informação de cada classe */

        WINDOW *info = malloc(sizeof(WINDOW));
        info = newwin(
                      g_renderstate->nrows/3                             , g_renderstate->ncols/2, 
                      g_renderstate->nrows/3 + ESPACAMENTO + BOTOES*2 + 2, g_renderstate->ncols/4
        );

        /* Retângulo à volta da janela */

        box(info, 0, 0);


        /* Colocação do texto com a descrição de cada classe */         // !!!!!!

        wattron(info, A_BOLD | A_UNDERLINE);


        switch(botao_selecionado) {
            
            case 0 : {
                mvwprintw(info, 1, 2, "Priest");
                wattroff(info, A_BOLD | A_UNDERLINE);
                // Print da descrição da class
                break;
            }
            
            case 1 : {
                mvwprintw(info, 1, 2, "AA");            // Nomes da classe a colocar !!!!!
                wattroff(info, A_BOLD | A_UNDERLINE);
                // Print da descrição da class
                break;
            }
            
            case 2 : {
                mvwprintw(info, 1, 2, "BB");            // Nomes da classe a colocar !!!!!
                wattroff(info, A_BOLD | A_UNDERLINE);
                // Print da descrição da class
                break;
            }

    }

        /* Atualização de ambas as janelas */

        wrefresh(menu->wnd);
        wrefresh(info);


    for (int i = 0; i < BOTOES; i++)
        free(botoes[i]);

    free(botoes);

}






void tick_CharactersMenu() {
    return;
}


void handle_CharactersMenu_keybinds(int key) {

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

                            case 0 : break;            //!!!!! Alterar na struct entity a classe para X
                            case 1 : break;             //!!!!! '' '' '' '' '' para Y
                            case 2 : break;             //!!!!!! '' '' '' '' '' para Z

                        }

    }



}