#include <stdlib.h>
#include <string.h>
#include "mainmenu.h"
#include <ncurses.h>


int mainmenu()
{
    initscr();
    cbreak();   // Impedir que teclas nao funcionem devido a necessitarem que seja pressionado o ENTER

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);   // Obter proporcoes da Window stdscr

    WINDOW * menuwin = newwin(30, xMax, yMax-30, 0);    // Altura, Comprimento, Yinicio, Xinicio
    box(menuwin, 0, 0); // Box temporario, apenas para ter nocao do espaco ocupado no terminal
    keypad(menuwin, TRUE);  // Input do teclado na Window menuwin ativo
    mousemask(ALL_MOUSE_EVENTS, NULL);  // Input do mouse ativo

    char logo[] = "Roguelite"; 
    mvprintw(1, xMax/2 - (strlen(logo)/2), "%s", logo);   // Necessario (strlen(logo)/2) para ficar totalmente centrado
    refresh();  // Necessario para aparecer o Logo

    char *escolhas[] = {"Start Game", "Tutorial", "Options", "Exit"};
    int tecla, selecionado = 0 /*Start Game*/, blink = 0;

    while(22)
    {
        for(int i = 0, separaEscolhas = 2 ; i < (int) (sizeof(escolhas)/sizeof(char *)) ; i++, separaEscolhas+= 3){
            if(i == blink)  
                wattron(menuwin, A_BLINK);  // Deixar a opcao default a piscar

            mvwprintw(menuwin, yMax/4 + separaEscolhas, xMax/2 - (strlen(escolhas[i])/2), "%s", escolhas[i]);  // Janela, Y, X, string
            refresh();
            wattroff(menuwin, A_BLINK); // Necessario para as escolhas nao ficarem todas a piscar
        }
        MEVENT event;
        tecla = wgetch(menuwin);

        if((selecionado == 0 && tecla == KEY_UP) || (selecionado == 3 && tecla == KEY_DOWN)){}  // Ignorar os casos especificos
        else{
            switch(tecla){
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

        if(tecla == 32 || tecla == 10){  // SPACE || ENTER
            switch(selecionado){
                case 0: //  Start Game
                // Falta definir para iniciar o jogo !!!!!!!!!!!!!!!!!!
                    break;
                case 1: // Tutorial
                // Falta definir para iniciar o tutorial !!!!!!!!!!!!!!
                    break;
                case 2: // Options
                // Falta definir para iniciar as Opcoes  !!!!!!!!!!!!!!
                    break;
                case 3: // Exit
                    goto end;   
            }
        }

        if(tecla == KEY_MOUSE && getmouse(&event) == OK){
            if(event.bstate & BUTTON1_PRESSED){
                
            }
        }
    }

end:
    endwin();
    return 0;
}