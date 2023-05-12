#include "deadmenu.h"
#include "util/ncurses.h"
#include "../../render.h"

#define BOTOES 3
#define LARGURA_DEATH 65
#define ALTURA_DEATH 39
#define LARGURA_DIED 84
#define ALTURA_DIED 8

static char *died[ALTURA_DIED] = {
        "Y88b   d88P  .d88888b.  888     888       8888888b. 8888888 8888888888 8888888b.      \0",
        " Y88b d88P  d88P\" \"Y88b 888     888       888  \"Y88b  888   888        888  \"Y88b     \0",
        "  Y88o88P   888     888 888     888       888    888  888   888        888    888     \0",
        "   Y888P    888     888 888     888       888    888  888   8888888    888    888     \0",
        "    888     888     888 888     888       888    888  888   888        888    888     \0",
        "    888     888     888 888     888       888    888  888   888        888    888     \0",
        "    888     Y88b. .d88P Y88b. .d88P       888  .d88P  888   888        888  .d88P     \0",
        "    888      \"Y88888P\"   \"Y88888P\"        8888888P\" 8888888 8888888888 8888888P\"      \0"
};

static char *death[ALTURA_DEATH] = {
        "                                           .\"\"--.._             \0",
        "                                           []      `'--.._      \0",
        "                                           ||__           `'-,  \0",
        "                                         `)||_ ```'--..       \\ \0",
        "                     _                    #|##}        ``--._  |\0",
        "                  .'` `'.                #####}              `\\/\0",
        "                 /  .\"\"\".\\              ##{###                  \0",
        "                /  /_  _`\\\\            ## `||                   \0",
        "                | |(_)(_)||          _##   ||                   \0",
        "                | |  /\\  )|        _/##\\   ||                   \0",
        "                | |L====J |       / |# |   ||                   \0",
        "               /  /'-..-' /    .'`  \\  |   ||                   \0",
        "              /   |  :: | |_.-`      |  \\  ||                   \0",
        "             /|   `\\-::.| |          \\   | ||                   \0",
        "           /` `|   /    | |          |   / ||                   \0",
        "         |`    \\   |    / /          \\  |  ||                   \0",
        "        |       `\\_|    |/      ,.__. \\ |  ||                   \0",
        "        /                     /`    `\\ ||  ||                   \0",
        "       |           .         /        \\||  ||                   \0",
        "       |                     |         |/  ||                   \0",
        "       /         /           |         (   ||                   \0",
        "      /          .           /          )  ||                   \0",
        "     |            \\          |             ||                   \0",
        "    /             |          /             ||                   \0",
        "   |\\            /          |              ||                   \0",
        "   \\ `-._       |           /              ||                   \0",
        "    \\ ,##`\\    /`           |              ||                   \0",
        "     #####  \\  |            \\              ||                   \0",
        "    #### # |__/             |              ||                   \0",
        "    #### `.(                |              ||                   \0",
        "    `##` /`                 /              ||                   \0",
        "       /`                   /              ||                   \0",
        "      /                     |              ||                   \0",
        "     |                      \\              ||                   \0",
        "    /                        |             ||                   \0",
        "  /`                          \\            ||                   \0",
        "/`                            |            ||                   \0",
        "`-.___,-.      .-.        ___,'            ||                   \0",
        "         `---'`   `'----'`                                      \0"
}; 

static unsigned short int effect = 0, botao_selecionado_principal = 0;
static char *botoes[BOTOES] = {"menu.dead.new_game", "menu.dead.options", "menu.dead.quit"};



void drawDeadMenu(Menu menu) {
    

    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


    /* Obter onde vai ser colocada a ASCII - Death e a ASCII - Died em x */

    unsigned short int x_death = xMAX - LARGURA_DEATH*2.5;

    unsigned short int x_died = xMAX  - LARGURA_DIED*1.1;



    /* Printer da ASCII - Death */

    for(int i = 0 ; i < ALTURA_DEATH ; i++) {

        for(int j = 0 ; j < LARGURA_DEATH ; j++) {

            static unsigned short int pair = 0;

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

    for(int i = 0 ; i < ALTURA_DIED ; i++) {

        for(int j = 0 ; j < LARGURA_DIED ; j++) {

            static unsigned short int pair = 0;

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
     LARGURA_DIED/2 para chegar ao meio , uma vez que o meio localiza-se na palavra DIED, portanto fica estéticamente feio */
    
    rectangle(menu->wnd, 
              yMAX/2            , x_died + LARGURA_DIED/2 - tamanhoBotaoMaior/2 - 3 - 1,
              yMAX/2  + BOTOES*2, x_died + LARGURA_DIED/2 + tamanhoBotaoMaior/2 - 3
    );


    /* Print dos botões com effect A_REVERSE no que está selecionado 
     +1 necessário no mvwprintw, devido aos arredondamentos para o floor e o -3 devido ao mesmo motivo que para o rectangle (acima) */

    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == effect) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, yMAX/2 + separador + i +1, x_died + LARGURA_DIED/2 - 3 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2, "%s", get_localized_string(g_renderstate->language, botoes[i]));

        if(i == effect)
            wattroff(menu->wnd, A_REVERSE);
            
    }

}


void tick_DeadMenu() {
    return;
}

void handle_DeadMenu_keybinds(int key) {


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
                                // aqui seria a startGame(), igual à função que está no main menu
                                break;

                            case 1 :
                                displayMenu(MENU_OPTIONS);
                                break;

                            case 2 :
                                // aqui falta acabar o jogo e, como estamos dentro de um jogo, o PATH dos menus esta limpo, portanto vai dar para dar display
                                displayMenu(MENU_MAIN_MENU);    // do MENU_MAIN_MENU, coisa que nao acontece agora porque o PATH nao esta limpo e nos
                                break;      // estamos a aceder ao MENU_DEAD pelo MENU_MAIN_MENU
                        }

    }

}