#include "optionsmenu.h"
#include "util/ncurses.h"

#define BOTOES 3
#define LARGURA_OPTIONS 76
#define ALTURA_OPTIONS 8

static unsigned short int botao_selecionado_principal = 0, botao_selecionado_keybinder = 0;
static char *botoes[BOTOES] = {"menu.options.return", "menu.options.language", "menu.options.keybinds"};

static char *options[ALTURA_OPTIONS] = {
        " .d88888b.  8888888b. 88888888888 8888888 .d88888b.  888b    888  .d8888b. \0",
        "888     888 888    888    888       888  888     888 88888b  888 Y88b.     \0",
        "d88     88b 888   Y88b    888       888  d88     88b 8888b   888 d88P      \0",
        "888     888 888   d88P    888       888  888     888 888Y88b 888  \"Y888b.  \0",
        "888     888 8888888P\"     888       888  888     888 888 Y88b888     \"Y88b.\0",
        "888     888 888           888       888  888     888 888  Y88888       \"888\0",
        "Y88b. .d88P 888           888       888  Y88b. .d88P 888   Y8888 Y88b  d88P\0",
        " \"Y88888P\"  888           888     8888888 \"Y88888P\"  888    Y888  \"Y8888P\" \0"
};


void draw_OptionsMenu(Menu menu) {
    
    // Get the width of the widest button
    char *botaoMaior = tamanho_maxPalavra(BOTOES, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


    // Get where the ASCII ART will be placed
    unsigned short int x_options = xMAX/2 - LARGURA_OPTIONS/2;
    unsigned short int y_options = yMAX   - ALTURA_OPTIONS*4/3;

    // Create the rectangle that connects the ASCII ART and the Logo
    rectangle(menu->wnd, 
              ALTURA_LOGO                 , x_options - ALTURA_OPTIONS,
              y_options + ALTURA_OPTIONS/2, x_options + LARGURA_OPTIONS + ALTURA_OPTIONS
    );
    
    // Prints the ASCII stored in the array options
    wattron(menu->wnd, A_BOLD);

    for(int i = 0 ; i < ALTURA_OPTIONS ; i++)
        for(int j = 0 ; j < LARGURA_OPTIONS ; j++)
            mvwprintw(menu->wnd, y_options + i, x_options + j, "%c", options[i][j]);

    wattroff(menu->wnd, A_BOLD);

    // Create the rectangle around the buttons
    rectangle(menu->wnd, 
        g_renderstate->nrows/2 - ALTURA_OPTIONS/2           , g_renderstate->ncols/2 - tamanhoBotaoMaior/2 - 1, 
        g_renderstate->nrows/2 - ALTURA_OPTIONS/2 + BOTOES*2, g_renderstate->ncols/2 + tamanhoBotaoMaior/2 
    );

    // Print the logo
    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);

    // Prints the buttons (The selected one is highlighted)
    for(int i = 0, separador = 0 ; i < BOTOES ; i++, separador += 1) {

        if(i == botao_selecionado_principal) wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, 
            g_renderstate->nrows/2 - ALTURA_OPTIONS/2 + separador + i + 1 ,
            g_renderstate->ncols/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2,
            "%s", get_localized_string(g_renderstate->language, botoes[i])
        );
        
        if(i == botao_selecionado_principal) wattroff(menu->wnd, A_REVERSE);

    }

}

void tick_OptionsMenu() {
    return;
}

void handle_OptionsMenu_keybinds(int key) {
    if(botao_selecionado_principal == 0 && key == KEY_UP) {
        botao_selecionado_principal = BOTOES - 1;
        // effect = BOTOES - 1;

        return;
    }

    if(botao_selecionado_principal == BOTOES-1 && key == KEY_DOWN) {
        botao_selecionado_principal = 0;
        // effect = 0;

        return;
    }
    
    switch(key) {
        case KEY_UP :
            botao_selecionado_principal--;
            // effect--;
            break;

        case KEY_DOWN :
            botao_selecionado_principal++;
            // effect++;
            break;

        case 10 : case 13 : 
            switch(botao_selecionado_principal) {
                case 0 :
                    closeMenu(MENU_OPTIONS);
                    break;
                case 1 :
                    change_locale();
                    break;
                case 2 :
                    displayMenu(MENU_OPTIONS_KEYBINDER);
                    break;
                                
            }
            break;
        case 'Q':
        case 'q':
            closeMenu(MENU_OPTIONS);
            break;
    }
}

void cleanup_options_menu() {
    botao_selecionado_principal = 0;
}

void draw_OptionsKeybinderMenu(Menu menu){

    static char *keybinder[] = {
        "menu.options.keybinder.return", 
        "menu.options.keybinder.up", 
        "menu.options.keybinder.down", 
        "menu.options.keybinder.left", 
        "menu.options.keybinder.right", 
        "menu.options.keybinder.projectile", 
        "menu.options.keybinder.trap",
        "menu.options.keybinder.molotov",
        "menu.options.keybinder.ultimate",
        "menu.options.keybinder.pause",
        "menu.options.keybinder.console"
    };

    box(menu->wnd, 0 , 0);


    for(int i = 0, j = 0, separador = 0; i < BOTOES_KEYBINDER/2 + 1 ; i++, separador++) {

        if(i == botao_selecionado_keybinder) 
            wattron(menu->wnd, A_REVERSE);
        
        if(i == 0) {
            mvwprintw(menu->wnd, 
                    1,
                    LARGURA_OPTIONS_KEYBINDER - strlen(get_localized_string(g_renderstate->language, keybinder[i]))/2,
                    "%s", get_localized_string(g_renderstate->language, keybinder[0])
                    );
        }

        else {
            mvwprintw(menu->wnd, 
              3 + separador + j,
              LARGURA_OPTIONS_KEYBINDER/4,
              "%s  -  %c", get_localized_string(g_renderstate->language, keybinder[i]), keybinds[j]
            );
        }
    
        if(i == botao_selecionado_keybinder)
            wattroff(menu->wnd, A_REVERSE);
        
        if(i != 0 && j < KEYBINDS)
            j++;
    }

    for(int i = BOTOES_KEYBINDER/2 + 1, separador = 1, j = 0 ; i < BOTOES_KEYBINDER ; i++, separador++, j++) {
        
        if(i == botao_selecionado_keybinder) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, 
              3 + separador + j,
              LARGURA_OPTIONS_KEYBINDER*1.2,
              "%s  -  %c", get_localized_string(g_renderstate->language, keybinder[i]), keybinds[i-1]
            );
        
        if(i == botao_selecionado_keybinder)
            wattroff(menu->wnd, A_REVERSE);
    }


}

void tick_OptionsKeybinderMenu() {
    return;
}
void handle_OptionsKeybinderMenu_keybinds(int key){

    if(botao_selecionado_keybinder == 0 && key == KEY_UP) {

        botao_selecionado_keybinder = BOTOES_KEYBINDER - 1;
        return;
    }

    if(botao_selecionado_keybinder == BOTOES_KEYBINDER-1 && key == KEY_DOWN) {

        botao_selecionado_keybinder = 0;
        return;
    }
    
    switch(key) {
        case KEY_UP :
            botao_selecionado_keybinder--;
            break;

        case KEY_DOWN :
            botao_selecionado_keybinder++;
            break;

        case 10 : case 13 : 
            unsigned char temp;
            switch(botao_selecionado_keybinder) {
                case 0 : 
                    closeMenu(MENU_OPTIONS_KEYBINDER);
                    break;

                case 1 : 
                    scanf("%c", &temp);
                    keybinds[0] = (int) temp;
                    break;

                case 2 : 
                    scanf("%c", &temp);
                    keybinds[1] = (int) temp;
                    break;

                case 3 : 
                    scanf("%c", &temp);
                    keybinds[2] = (int) temp;
                    break;

                case 4 :
                    scanf("%c", &temp);
                    keybinds[3] = (int) temp;
                    break;
                
                case 5 :
                    scanf("%c", &temp);
                    keybinds[4] = (int) temp;
                    break;
                
                case 6 : 
                    scanf("%c", &temp);
                    keybinds[5] = (int) temp;
                    break;
                
                case 7 :
                    scanf("%c", &temp);
                    keybinds[6] = (int) temp;
                    break;

                case 8 :
                    scanf("%c", &temp);
                    keybinds[7] = (int) temp;
                    break;
                case 9 :
                    scanf("%c", &temp);
                    keybinds[8] = (int) temp;
                    break;
                case 10 :
                    scanf("%c", &temp);
                    keybinds[9] = (int) temp;
                    break;

            }

            break;

    }
    
}

void cleanup_options_keybinder(){
    botao_selecionado_keybinder = 0;
    return;
}