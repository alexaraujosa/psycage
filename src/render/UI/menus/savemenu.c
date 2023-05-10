#include "savemenu.h"
#include "util/ncurses.h"
#include "../../render.h"
#include <stdlib.h>

#define BOTOES_PRINCIPAL 4
#define BOTOES_INFOS 3
#define ESPACAMENTO 2

static unsigned short int effect_principal = 0, botao_selecionado_principal = 0, save_selecionado = 0, effect_infos = 0, botao_selecionado_infos = 0;
static char *botoes[BOTOES_PRINCIPAL] = {"menu.save.return", "menu.save.slot1", "menu.save.empty_slot", "menu.save.slot3"};
static char *botoes_infos[BOTOES_INFOS] = {"menu.save.infos.return", "menu.save.infos.save", "menu.save.infos.load"};



/*

verificarExisteSave





if(verificarExisteSave(save1) == 1)
    botoes[1] = get_localized_string(EN_US, "menu.save.slot1");
*/










void drawSaveInfo(Menu menu) {

    box(menu->wnd, 0, 0);



    for(int i = 0, separador = 0 ; i < BOTOES_INFOS ; i++, separador += 1) {

        if(i == effect_infos) 
            wattron(menu->wnd, A_BOLD | A_DIM | A_REVERSE);
        
        mvwprintw(menu->wnd, getmaxy(menu->wnd)/4 + separador + i +1 , getmaxx(menu->wnd)/2 - strlen(get_localized_string(g_renderstate->language, botoes_infos[i]))/2, "%s", get_localized_string(g_renderstate->language, botoes_infos[i]));

        if(i == effect_infos)
            wattroff(menu->wnd, A_BOLD | A_DIM | A_REVERSE);
            
    }

}






void drawSaveMenu(Menu menu) {


    char *botaoMaior = tamanho_maxPalavra(BOTOES_PRINCIPAL, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);


    /* Cria o retângulo à volta dos botões */
    
    rectangle(menu->wnd, 
              yMAX*0.3           , xMAX/2 - tamanhoBotaoMaior/2 - 1,
              yMAX*0.3 + BOTOES_PRINCIPAL*2, xMAX/2 + tamanhoBotaoMaior/2 
    );


    /* Print do logo */

    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);


    for(int i = 0, separador = 0 ; i < BOTOES_PRINCIPAL ; i++, separador += 1) {

        if(i == effect_principal) 
            wattron(menu->wnd, A_REVERSE);
        
        mvwprintw(menu->wnd, yMAX*0.3 + separador + i +1 , xMAX/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2, "%s", get_localized_string(g_renderstate->language, botoes[i]));

        if(i == effect_principal)
            wattroff(menu->wnd, A_REVERSE);
            
    }



}

void tick_SaveMenu() {
    return;
}

void handle_SaveMenu_keybinds(int key) {

if(save_selecionado == 0) {

        if(botao_selecionado_principal == 0 && key == KEY_UP) {

            botao_selecionado_principal = BOTOES_PRINCIPAL - 1;
            effect_principal = BOTOES_PRINCIPAL - 1;

            return;
        }

        if(botao_selecionado_principal == BOTOES_PRINCIPAL-1 && key == KEY_DOWN) {

            botao_selecionado_principal = 0;
            effect_principal = 0;

            return;
        }
    
        switch(key) {

            case KEY_UP :
                botao_selecionado_principal--;
                effect_principal--;
                break;


            case KEY_DOWN :
                botao_selecionado_principal++;
                effect_principal++;
                break;


            case 10 : case 13 : switch(botao_selecionado_principal) {

                                case 0 : 
                                    closeMenu(MENU_SAVE);
                                    break;

                                case 1 : 
                                    displayMenu(MENU_SAVE_SLOT);    // Apenas nao pode aceder se estiver no Menu Principal e o save for vazio,
                                    save_selecionado = 1;           // Se existir save, ele pode querer entrar nesse jogo
                                    break;

                                case 2 : break; //save2

                                case 3 : break; //save3

                            }

        }
}
else {

        if(botao_selecionado_infos == 0 && key == KEY_UP) {

            botao_selecionado_infos = BOTOES_INFOS - 1;
            effect_infos = BOTOES_INFOS - 1;

            return;
        }

        if(botao_selecionado_infos == BOTOES_INFOS-1 && key == KEY_DOWN) {

            botao_selecionado_infos = 0;
            effect_infos = 0;

            return;
        }

        switch(key) {

            case KEY_UP :
                botao_selecionado_infos--;
                effect_infos--;
                break;


            case KEY_DOWN :
                botao_selecionado_infos++;
                effect_infos++;
                break;


            case 10 : case 13 : switch(botao_selecionado_infos) {

                                case 0 : 
                                    closeMenu(MENU_SAVE_SLOT);
                                    save_selecionado = 0;
                                    break;

                                case 1 : 
                                    displayMenu(MENU_SAVE_SLOT); 
                                    break; //dar save

                                case 2 : 
                                    break; //dar load


                            }

        }

}





}