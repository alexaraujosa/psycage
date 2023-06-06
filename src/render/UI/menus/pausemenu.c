#include "pausemenu.h"
#include "util/ncurses.h"

#define BOTOES 4
#define LARGURA_PAUSE 59
#define ALTURA_PAUSE 8

static unsigned short int botao_selecionado_principal = 0;
static char *botoes[BOTOES] = {"menu.pause.return", "menu.pause.save", "menu.pause.options", "menu.pause.exit"};

static char *pausa[ALTURA_PAUSE] = {
        "  8888888b.      d8888 888     888  .d8888b.  8888888888  \0",
        "  888   Y88b    d88888 888     888 d88P  Y88b 888         \0",
        "  888    888   d88P888 888     888 Y88b.      888         \0",
        "  888   d88P  d88P 888 888     888  \"Y888b.   8888888     \0",  
        "  8888888P\"  d88P  888 888     888     \"Y88b. 888         \0",   
        "  888       d88P   888 888     888       \"888 888         \0",  
        "  888      d8888888888 Y88b. .d88P Y88b  d88P 888         \0", 
        "  888     d88P     888  \"Y88888P\"   \"Y8888P\"  8888888888  \0" 
                                                       
};




void draw_PauseMenu(Menu menu) {

    // Get the width of the widest button
    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);

    // Get where the ASCII ART will be placed
    unsigned short int x_pause = xMAX/2 - LARGURA_PAUSE/2;
    unsigned short int y_pause = yMAX   - ALTURA_PAUSE*4/3;

    // Create the rectangle that connects the ASCII ART and the Logo
    rectangle(menu->wnd,
              ALTURA_LOGO                , x_pause - ALTURA_PAUSE, 
              y_pause + ALTURA_PAUSE/2 -1, x_pause + LARGURA_PAUSE + ALTURA_PAUSE
    );

    // Prints the ASCII stored in the array pause in bold
    wattron(menu->wnd, A_BOLD);

    for(int i = 0 ; i < ALTURA_PAUSE ; i++)
        for(int j = 0 ; j < LARGURA_PAUSE ; j++)
            mvwprintw(menu->wnd, y_pause + i, x_pause + j + i, "%c", pausa[i][j]);

    wattroff(menu->wnd, A_BOLD);

    // Create the rectangle around the buttons
    rectangle(
              menu->wnd, 
              yMAX/2 - ALTURA_PAUSE/2           , xMAX/2 - tamanhoBotaoMaior/2 - 1, 
              yMAX/2 - ALTURA_PAUSE/2 + BOTOES*2, xMAX/2 + tamanhoBotaoMaior/2 
             );

    // Print the logo
    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);

    // Prints the buttons (The selected one is highlighted)
    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == botao_selecionado_principal)
            wattron(menu->wnd, A_REVERSE);

        mvwprintw(menu->wnd, 
                  yMAX/2 - ALTURA_PAUSE/2 + separador + i + 1 ,
                  xMAX/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2,
                  "%s", get_localized_string(g_renderstate->language, botoes[i])
                );
        
        if(i == botao_selecionado_principal)
            wattroff(menu->wnd, A_REVERSE);

    }

}

void tick_PauseMenu() {
    return;
}


void handle_PauseMenu_keybinds(int key) {


    if(botao_selecionado_principal == 0 && key == KEY_UP) {

        botao_selecionado_principal = BOTOES - 1;

        return;
    }

    if(botao_selecionado_principal == BOTOES-1 && key == KEY_DOWN) {

        botao_selecionado_principal = 0;

        return;
    }
    
    switch(key) {

        case KEY_UP :
            botao_selecionado_principal--;
            break;


        case KEY_DOWN :
            botao_selecionado_principal++;
            break;


        case 10 : case 13 : switch(botao_selecionado_principal) {

                            case 0 : 
                                closeMenu(MENU_PAUSE);
                                break;
                            
                            case 1 :
                                displayMenu(MENU_SAVE);
                                break;

                            case 2 : 
                                displayMenu(MENU_OPTIONS); 
                                break;
                            case 3 : 
                                closeMenu(g_renderstate->menus[0]->id);
                                displayMenu(MENU_MAIN_MENU); 
                                break;

                        }

    }
    
}

void cleanup_pause_menu() {
    botao_selecionado_principal = 0;
}
