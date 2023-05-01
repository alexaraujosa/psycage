#include "deadmenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 3

static char *botoes[] = {"   New   Game   ", "    Settings    ", "      Exit      "};
static int botao_selecionado = 0, effect = 0;

void drawDeadMenu(Menu menu) {

    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    int tamanhoBotaoMaior = strlen(botaoMaior);


    static char *death[] = {
        "                                           .\"\"--.._             ",
        "                                           []      `'--.._      ",
        "                                           ||__           `'-,  ",
        "                                         `)||_ ```'--..       \\ ",
        "                     _                    #|##}        ``--._  |",
        "                  .'` `'.                #####}              `\\/",
        "                 /  .\"\"\".\\              ##{###                  ",
        "                /  /_  _`\\\\            ## `||                   ",
        "                | |(_)(_)||          _##   ||                   ",
        "                | |  /\\  )|        _/##\\   ||                   ",
        "                | |L====J |       / |# |   ||                   ",
        "               /  /'-..-' /    .'`  \\  |   ||                   ",
        "              /   |  :: | |_.-`      |  \\  ||                   ",
        "             /|   `\\-::.| |          \\   | ||                   ",
        "           /` `|   /    | |          |   / ||                   ",
        "         |`    \\   |    / /          \\  |  ||                   ",
        "        |       `\\_|    |/      ,.__. \\ |  ||                   ",
        "        /                     /`    `\\ ||  ||                   ",
        "       |           .         /        \\||  ||                   ",
        "       |                     |         |/  ||                   ",
        "       /         /           |         (   ||                   ",
        "      /          .           /          )  ||                   ",
        "     |            \\          |             ||                   ",
        "    /             |          /             ||                   ",
        "   |\\            /          |              ||                   ",
        "   \\ `-._       |           /              ||                   ",
        "    \\ ,##`\\    /`           |              ||                   ",
        "     #####  \\  |            \\              ||                   ",
        "    #### # |__/             |              ||                   ",
        "    #### `.(                |              ||                   ",
        "    `##` /`                 /              ||                   ",
        "       /`                   /              ||                   ",
        "      /                     |              ||                   ",
        "     |                      \\              ||                   ",
        "    /                        |             ||                   ",
        "  /`                          \\            ||                   ",
        "/`                            |            ||                   ",
        "`-.___,-.      .-.        ___,'            ||                   ",
        "         `---'`   `'----'`                                      "
    };  

    static char *died[] = {
        "Y88b   d88P  .d88888b.  888     888       8888888b. 8888888 8888888888 8888888b.      ",
        " Y88b d88P  d88P\" \"Y88b 888     888       888  \"Y88b  888   888        888  \"Y88b ",
        "  Y88o88P   888     888 888     888       888    888  888   888        888    888     ",
        "   Y888P    888     888 888     888       888    888  888   8888888    888    888     ",
        "    888     888     888 888     888       888    888  888   888        888    888     ",
        "    888     888     888 888     888       888    888  888   888        888    888     ",
        "    888     Y88b. .d88P Y88b. .d88P       888  .d88P  888   888        888  .d88P     ",
        "    888      \"Y88888P\"   \"Y88888P\"        8888888P\" 8888888 8888888888 8888888P\""
    };


    /* Obter altura e largura da ASCII - Death e da ASCII - Died */

    int altura_death = sizeof(death) / sizeof(death[0]);
    int largura_death = strlen(death[0]);

    int altura_died = sizeof(died) / sizeof(died[0]);
    int largura_died = strlen(died[1]);


    /* Obter onde vai ser colocada a ASCII - Death e a ASCII - Died em x */

    int x_death = g_renderstate->ncols - largura_death*2.5;

    int x_died = g_renderstate->ncols  - largura_died*1.1;



    /* Printer da ASCII - Death */

    for(int i = 0 ; i < altura_death ; i++) {

        for(int j = 0 ; j < largura_death ; j++) {

            int pair = 0;

            switch(death[i][j]) {
                case '#' : case '{' : case '}' : pair = LIGHTPLUS_GREY_LOGO; break;
                default : pair = DARKPLUS_GREY_LOGO;
            }

            wattron(menu->wnd, COLOR_PAIR(pair));

            mvwprintw(menu->wnd, 3 + i, x_death + j, "%c", death[i][j]);

            wattroff(menu->wnd, COLOR_PAIR(pair));
        }

    }


    /* Printer da ASCII - Died */

    for(int i = 0 ; i < altura_died ; i++) {

        for(int j = 0 ; j < largura_died ; j++) {

            int pair = 0;

            switch(died[i][j]) {
                case '"' : case '.' : pair = LIGHTPLUS_GREY_LOGO; break;
                default : pair = DARKPLUS_GREY_LOGO;
            }

            wattron(menu->wnd, COLOR_PAIR(pair));

            mvwprintw(menu->wnd, 3 + i, x_died + j, "%c", died[i][j]);

            wattroff(menu->wnd, COLOR_PAIR(pair));
        }

    }


    /* Cria o retângulo à volta dos botões. O -3 é necessário, porque a largura do YOU e do DIED são diferentes, logo não podemos usar 
     largura_died/2 para chegar ao meio , uma vez que o meio localiza-se na palavra DIED, portanto fica estéticamente feio */
    
    rectangle(menu->wnd, 
              g_renderstate->nrows/2            , x_died + largura_died/2 - tamanhoBotaoMaior/2 - 3 - 1,
              g_renderstate->nrows/2  + BOTOES*2, x_died + largura_died/2 + tamanhoBotaoMaior/2 - 3
    );


    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor e o -3 devido ao mesmo motivo que para o rectangle (acima) */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, g_renderstate->nrows/2 + separador + i +1, x_died + largura_died/2 - 3 - strlen(botoes[i])/2, "%s", botoes[i]);

        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);
            
    }

}


void tick_DeadMenu() {
    return;
}

void handle_DeadMenu_keybinds(int key) {

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
                            case 1 : break; //options
                            case 2 : break; //ir para o menu principal

                        }

    }

}