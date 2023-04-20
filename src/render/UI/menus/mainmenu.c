#include "mainmenu.h"

static char* escolhas[] = {"Start Game", "Tutorial", "Options", "Exit"};
static int selecionado = 0 /*Start Game*/, blink = 0;

void drawMainMenu(Menu menu) {
    box(menu->wnd, 0, 0);

    char logo[] = "Roguelite"; 
    mvwprintw(menu->wnd, 1, g_renderstate->ncols/2 - (strlen(logo)/2), "%s", logo);

    for(int i = 0, separaEscolhas = 2 ; i < (int) (sizeof(escolhas)/sizeof(char *)) ; i++, separaEscolhas+= 3){
        if(i == blink) wattron(menu->wnd, A_BLINK);  // Deixar a opcao default a piscar
            
        mvwprintw(
            menu->wnd, 
            g_renderstate->nrows/4 + separaEscolhas, 
            g_renderstate->ncols/2 - (strlen(escolhas[i])/2), 
            "%s", 
            escolhas[i]
        ); // Janela, Y, X, string
        refresh();
        wattroff(menu->wnd, A_BLINK); // Necessario para as escolhas nao ficarem todas a piscar
    }
}

void tick_MainMenu() {
    return;
}

void handle_MainMenu_Keybinds(int key) {
    if((selecionado == 0 && key == KEY_UP) || (selecionado == 3 && key == KEY_DOWN)) {}
    else {
        switch(key) {
            case KEY_UP:
                blink--;
                selecionado--;
                break;
            case KEY_DOWN:
                blink++;
                selecionado++;
                break;
            default:
                break;
        }   
    }
}