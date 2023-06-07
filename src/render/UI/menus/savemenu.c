#include "savemenu.h"
#include "util/ncurses.h"
#include "common.h"

#define BOTOES_PRINCIPAL 4
#define BOTOES_INFOS 4
#define ESPACAMENTO 2

static unsigned short int 
    // effect_principal = 0, 
    botao_selecionado_principal = 0, 
    save_selecionado = FALSE, 
    // effect_infos = 0, 
    botao_selecionado_infos = 0;

static char *botoes[BOTOES_PRINCIPAL] = {"menu.save.return", "menu.save.slot", "menu.save.slot", "menu.save.slot"};
static char *botoes_infos[BOTOES_INFOS] = {
    "menu.save.infos.return", 
    "menu.save.infos.save", 
    "menu.save.infos.load", 
    "menu.save.infos.delete"
};
char* dtext = NULL;

// Cache system
struct sm_cache_node {
    int args;
    char* value;
};

static int cached = FALSE;
static DataLocale cur_loc = EN_US;

#define CACHE_NUM 2 + BOTOES_PRINCIPAL + BOTOES_INFOS + 11
static struct sm_cache_node cached_translations[CACHE_NUM];

struct sm_cache_node _sm_make_cache(int args, char* value) {
    struct sm_cache_node node = {
        args,
        value
    };

    return node;
}
int test_ct = CACHE_NUM;

/*
 * This function saves all translations in an array to avoid multiple memory allocations without deallocations.
 *
 * A.k.a: This function is everything that makes this menu run (slightly) faster than a handicap.
 */
void _sm_ensure_cache() {
    if (!cached || cur_loc != g_renderstate->language) {
        if (cached) {
            for (int i = 0; i < CACHE_NUM; i++) {
                if (cached_translations[i].args) free(cached_translations[i].value);
            }
        }

        cached_translations[0] = _sm_make_cache(
            // TRUE, get_args_localized_string(g_renderstate->language, "menu.save.slot", botao_selecionado_principal)
            TRUE, get_localized_string(g_renderstate->language, "menu.save.slot")
        );
        cached_translations[1] = _sm_make_cache(
            FALSE, get_localized_string(g_renderstate->language, "menu.save.infos.not_available")
        );

        // Botões Save Info
        for(int i = 0; i < BOTOES_INFOS; i++) {
            cached_translations[2 + i] = _sm_make_cache(
                FALSE, get_localized_string(g_renderstate->language, botoes_infos[i])
            );
        }

        // Botões Save
        for(int i = 0; i < BOTOES_PRINCIPAL; i++) {
            cached_translations[2 + BOTOES_INFOS + i] = _sm_make_cache(
                TRUE, get_args_localized_string(g_renderstate->language, botoes[i], i)
            );
        }

        // Dialogos
        char* dialog_text = NULL;
        char* orig_text = NULL;

        // - General
        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.not_found");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 0] = _sm_make_cache(
            TRUE, dialog_text
        );

        // - Deletion
        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.deleting");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 1] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.delete_failure");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 2] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.delete_success");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 3] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.delete_confirm");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 4] = _sm_make_cache(
            TRUE, dialog_text
        );

        // - Saving
        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.saving");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 5] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.save_failure");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 6] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.save_success");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 7] = _sm_make_cache(
            TRUE, dialog_text
        );

        // - Loading
        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.loading");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 8] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.load_failure");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 9] = _sm_make_cache(
            TRUE, dialog_text
        );

        orig_text = get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.load_success");
        dialog_text = (char*)malloc(strlen(orig_text)*sizeof(char));
        strncpy(dialog_text, orig_text, strlen(orig_text));
        // replace_substring(dialog_text, "{__NL}", "\n");
        cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 10] = _sm_make_cache(
            TRUE, dialog_text
        );

        cached = TRUE;
    }
}

void draw_SaveInfo(Menu menu) {
    // Creates a rectangle around the window
    box(menu->wnd, 0, 0);

    // Handle Cache
    _sm_ensure_cache();

    char* _title_transl = cached_translations[0].value;
    int transl_size = strlen(_title_transl) + 1;

    char* title_transl = (char*)malloc(transl_size);
    snprintf(title_transl, transl_size, _title_transl, botao_selecionado_principal);

    mvwprintw(menu->wnd,
        2,
        getmaxx(menu->wnd)/2 - strlen(title_transl)/2,
        "%s", title_transl
    );

    // Prints the buttons (The selected one is highlighted)
    for(int i = 0, separador = 0 ; i < BOTOES_INFOS ; i++, separador += 1) {
        if(i == botao_selecionado_infos) wattron(menu->wnd, A_BOLD | A_DIM | A_REVERSE);

        char* transl = NULL;
        if (!g_gamestate->input_initialized && i == 1) {
            transl = cached_translations[1].value;
        } else if (i > 1) {
            if (verify_Save(botao_selecionado_principal)) {
                transl = cached_translations[1].value;
            } else {
                transl = cached_translations[2 + i].value;
            }
        } else {
            transl = cached_translations[2 + i].value;
        }
        
        
        mvwprintw(menu->wnd, 
            getmaxy(menu->wnd)/4 + separador + i + 2,
            getmaxx(menu->wnd)/2 - strlen(transl)/2,
            "%s", transl
        );

        // if(i == effect_infos) wattroff(menu->wnd, A_BOLD | A_DIM | A_REVERSE);  
        if(i == botao_selecionado_infos) wattroff(menu->wnd, A_BOLD | A_DIM | A_REVERSE);  
    }
}

void draw_SaveMenu(Menu menu) {
    // Get the width of the widest button
    char *botaoMaior = tamanho_maxPalavra(BOTOES_PRINCIPAL, botoes);
    unsigned short int tamanhoBotaoMaior = strlen(botaoMaior);

    // Create the rectangle around the buttons
    rectangle(menu->wnd, 
        yMAX*0.3                     , xMAX/2 - tamanhoBotaoMaior/2 - 1,
        yMAX*0.3 + BOTOES_PRINCIPAL*2, xMAX/2 + tamanhoBotaoMaior/2 
    );

    // Handle Cache
    _sm_ensure_cache();

    // Print the logo
    printer(menu->wnd, yMAX/4 - ALTURA_LOGO, xMAX/2 - LARGURA_LOGO/2);

    // The "printer" function screws up the memory in ways only God comprehends. Somehow.
    if (botao_selecionado_principal > BOTOES_PRINCIPAL) botao_selecionado_principal = 0;

    // Prints the buttons (The selected one is highlighted)
    for(int i = 0, separador = 0 ; i < BOTOES_PRINCIPAL ; i++, separador += 1) {

        // if(i == effect_principal) wattron(menu->wnd, A_REVERSE);
        if(i == botao_selecionado_principal) wattron(menu->wnd, A_REVERSE);
        
        // char* transl = get_args_localized_string(g_renderstate->language, botoes[i], i);
        char* transl = cached_translations[2 + BOTOES_INFOS + i].value;

        mvwprintw(menu->wnd,
            yMAX*0.3 + separador + i + 1,
            // xMAX/2 - strlen(get_localized_string(g_renderstate->language, botoes[i]))/2,
            // "%s", get_localized_string(g_renderstate->language, botoes[i])
            xMAX/2 - strlen(transl)/2,
            "%s", transl
        );

        // if(i == effect_principal) wattroff(menu->wnd, A_REVERSE); 
        if(i == botao_selecionado_principal) wattroff(menu->wnd, A_REVERSE); 
    }
}

void tick_SaveMenu() {
    return;
}

int hsmk_delete(int key) {
    free(dtext);

    switch (key) {
        case 'Y':
        case 'y': {
            g_dialog_keybinds = NULL;

            // g_dialog_text = "Deleting save.\nPlease do not close your game.\0";
            // dtext = (char*)malloc(40*2*sizeof(char));
            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.deleting"));
            // replace_substring(dtext, "{__NL}", "\n");

            // g_dialog_text = dtext;
            
            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 1].value;

            int gsl = 0, gsw = 0;
            get_multiline_stats(
                g_dialog_text,
                &gsl,
                &gsw
            );

            g_ui_size[0] = gsl + 4;
            g_ui_size[1] = gsw + 5;
            g_dialog_control[0] = gsw + 3;
            g_dialog_control[1] = gsl;

            // g_ui_size[0] = 5;
            // g_ui_size[1] = 40;
            // g_dialog_control[0] = 40;
            // g_dialog_control[1] = 2;
            g_dialog_control[3] = 0;

            char**** _page_data = malloc(sizeof(char****));
            int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

            g_dialog_control[2] = page_count;
            g_dialog_page_data = _page_data;

            hmrMenuCache(MENU_DIALOG);
            displayMenu(MENU_DIALOG);

            // free(dtext);
            if (delete_Save(botao_selecionado_principal)) {
                // g_dialog_text = "Unable to delete save.\nPress any key to continue.";
                // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.delete_failure"));
                g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 2].value;
            } else {
                // g_dialog_text = "Deleted save successfully.\nPress any key to continue.";
                // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.delete_success"));
                g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 3].value;
            }
            
            // replace_substring(dtext, "{__NL}", "\n");
            // g_dialog_text = dtext;

            gsl = 0, gsw = 0;
            get_multiline_stats(
                g_dialog_text,
                &gsl,
                &gsw
            );

            g_ui_size[0] = gsl + 4;
            g_ui_size[1] = gsw + 5;
            g_dialog_control[0] = gsw + 3;
            g_dialog_control[1] = gsl;

            g_dialog_control[3] = 1;
            page_count = calculate_dialog_metadata(g_dialog_text, _page_data);
            g_dialog_control[2] = page_count;
            g_dialog_page_data = _page_data;

            botao_selecionado_infos = 0;

            hmrMenuCache(MENU_DIALOG);

            return 0;
            break;
        }
        case 'N':
        case 'n':
            return 1;
            break;
        default:
            return 0;
    }
}

int hsmk_load(int key) {
    IGNORE_ARG(key);

    g_dialog_keybinds = NULL;
    g_gamestate->valid_state = TRUE;
    g_gamestate->input_initialized = TRUE;
    closeMenu(g_renderstate->menus[0]->id);

    return 0;
}

void handle_SaveMenu_keybinds(int key) {
    if(save_selecionado == 0) {
        if(botao_selecionado_principal == 0 && key == KEY_UP) {
            botao_selecionado_principal = BOTOES_PRINCIPAL - 1;
            // effect_principal = BOTOES_PRINCIPAL - 1;

            return;
        }

        if(botao_selecionado_principal == BOTOES_PRINCIPAL-1 && key == KEY_DOWN) {
            botao_selecionado_principal = 0;
            // effect_principal = 0;

            return;
        }
    
        switch(key) {
            case KEY_UP :
                botao_selecionado_principal--;
                // effect_principal--;
                break;

            case KEY_DOWN :
                botao_selecionado_principal++;
                // effect_principal++;
                break;

            case 10 : case 13 : 
                switch(botao_selecionado_principal) {
                    case 0: 
                        closeMenu(MENU_SAVE);
                        break;

                    case 1: 
                    case 2:
                    case 3:
                        displayMenu(MENU_SAVE_SLOT);    // Apenas nao pode aceder se estiver no Menu Principal e o save for vazio,
                        save_selecionado = TRUE;           // Se existir save, ele pode querer entrar nesse jogo
                        break;

                    // case 2 : break; //save2

                    // case 3 : break; //save3

                }
                break;
            case 'Q':
            case 'q':
                closeMenu(MENU_SAVE);
                save_selecionado = FALSE;
                break;
        }
    } else {
        if(botao_selecionado_infos == 0 && key == KEY_UP) {
            if (verify_Save(botao_selecionado_principal)) {
                if (!g_gamestate->input_initialized) botao_selecionado_infos = 0;
                else botao_selecionado_infos = 1;
                // effect_infos = 1;
            } else {
                botao_selecionado_infos = BOTOES_INFOS - 1;
                // effect_infos = BOTOES_INFOS - 1;
            }

            return;
        }

        if(botao_selecionado_infos == BOTOES_INFOS-1 && key == KEY_DOWN) {
            botao_selecionado_infos = 0;
            // effect_infos = 0;

            return;
        }

        switch(key) {
            case KEY_UP :
                botao_selecionado_infos--;
                // effect_infos--;
                break;

            case KEY_DOWN :
                if (verify_Save(botao_selecionado_principal)) {
                    // if (botao_selecionado_infos == 1) botao_selecionado_infos = 0;
                    if (!g_gamestate->input_initialized) botao_selecionado_infos = 0;
                    else if (botao_selecionado_infos == 1) botao_selecionado_infos = 0;
                    else botao_selecionado_infos++;;
                } else {
                    botao_selecionado_infos++;
                    // if (!g_gamestate->input_initialized && botao_selecionado_infos == 1) botao_selecionado_infos = 2;
                }
                break;

            case 10: case 13: 
                switch(botao_selecionado_infos) {
                    case 0: 
                        closeMenu(MENU_SAVE_SLOT);
                        save_selecionado = FALSE;
                        break;
                    case 1: {
                        // g_dialog_text = "Saving.\nPlease do not close your game.\0";
                        // dtext = (char*)malloc(40*2*sizeof(char));
                        // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.saving"));
                        // replace_substring(dtext, "{__NL}", "\n");

                        // g_dialog_text = dtext;

                        g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 5].value;

                        int gsl = 0, gsw = 0;
                        get_multiline_stats(
                            g_dialog_text,
                            &gsl,
                            &gsw
                        );

                        g_ui_size[0] = gsl + 4;
                        g_ui_size[1] = gsw + 5;
                        g_dialog_control[0] = gsw + 3;
                        g_dialog_control[1] = gsl;

                    	// g_ui_size[0] = 5;
                    	// g_ui_size[1] = 40;
                    	// g_dialog_control[0] = 40;
                    	// g_dialog_control[1] = 2;
                        g_dialog_control[3] = 0;

                    	char**** _page_data = malloc(sizeof(char****));
                    	int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

                    	g_dialog_control[2] = page_count;
                    	g_dialog_page_data = _page_data;
                    	displayMenu(MENU_DIALOG);

                        //free(dtext);
                        if (create_Save(botao_selecionado_principal)) {
                            // g_dialog_text = "Unable to save.\nPress any key to continue.";
                            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.save_failure"));
                            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 6].value;
                        } else {
                            // g_dialog_text = "Saved successfully.\nPress any key to continue.";
                            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.save_success"));
                            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 7].value;
                        }
                        
                        // replace_substring(dtext, "{__NL}", "\n");
                        // g_dialog_text = dtext;

                        gsl = 0, gsw = 0;
                        get_multiline_stats(
                            g_dialog_text,
                            &gsl,
                            &gsw
                        );

                        g_ui_size[0] = gsl + 4;
                        g_ui_size[1] = gsw + 5;
                        g_dialog_control[0] = gsw + 3;
                        g_dialog_control[1] = gsl;

                        g_dialog_control[3] = 1;
                        page_count = calculate_dialog_metadata(g_dialog_text, _page_data);
                        g_dialog_control[2] = page_count;
                    	g_dialog_page_data = _page_data;

                        hmrMenuCache(MENU_DIALOG);
                        
                        break;
                    }
                    case 2: {
                        if (verify_Save(botao_selecionado_principal)) {
                            // g_dialog_text = "Save file not found.\nPress any key to continue.\0";
                            // dtext = (char*)malloc(40*2*sizeof(char));
                            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.not_found"));
                            // replace_substring(dtext, "{__NL}", "\n");

                            // g_dialog_text = dtext;
                            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 0].value;

                            int gsl = 0, gsw = 0;
                            get_multiline_stats(
                                g_dialog_text,
                                &gsl,
                                &gsw
                            );

                            g_ui_size[0] = gsl + 4;
                            g_ui_size[1] = gsw + 5;
                            g_dialog_control[0] = gsw + 3;
                            g_dialog_control[1] = gsl;

                            // g_ui_size[0] = 5;
                            // g_ui_size[1] = 40;
                            // g_dialog_control[0] = 40;
                            // g_dialog_control[1] = 2;
                            g_dialog_control[3] = 1;

                            char**** _page_data = malloc(sizeof(char****));
                            int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

                            g_dialog_control[2] = page_count;
                            g_dialog_page_data = _page_data;
                            displayMenu(MENU_DIALOG);
                        } else {
                            // g_dialog_text = "Loading.\nPlease do not close your game.\0";
                            // dtext = (char*)malloc(40*2*sizeof(char));
                            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.loading"));
                            // replace_substring(dtext, "{__NL}", "\n");

                            // g_dialog_text = dtext;
                            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 8].value;

                            int gsl = 0, gsw = 0;
                            get_multiline_stats(
                                g_dialog_text,
                                &gsl,
                                &gsw
                            );

                            g_ui_size[0] = gsl + 4;
                            g_ui_size[1] = gsw + 5;
                            g_dialog_control[0] = gsw + 3;
                            g_dialog_control[1] = gsl;

                            // g_ui_size[0] = 5;
                            // g_ui_size[1] = 40;
                            // g_dialog_control[0] = 40;
                            // g_dialog_control[1] = 2;
                            g_dialog_control[3] = 0;

                            char**** _page_data = malloc(sizeof(char****));
                            int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

                            g_dialog_control[2] = page_count;
                            g_dialog_page_data = _page_data;
                            displayMenu(MENU_DIALOG);

                            // free(dtext);
                            if (load_save(botao_selecionado_principal)) {
                                // g_dialog_text = "Unable to load save.\nPress any key to continue.";
                                // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.load_failure"));
                                g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 9].value;
                            } else {
                                // g_dialog_text = "Save loaded successfully.\nPress any key to continue.";
                                // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.load_success"));
                                g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 10].value;
                            }
                        
                            // replace_substring(dtext, "{__NL}", "\n");
                            // g_dialog_text = dtext;

                            gsl = 0, gsw = 0;
                            get_multiline_stats(
                                g_dialog_text,
                                &gsl,
                                &gsw
                            );

                            g_ui_size[0] = gsl + 4;
                            g_ui_size[1] = gsw + 5;
                            g_dialog_control[0] = gsw + 3;
                            g_dialog_control[1] = gsl;
                            g_dialog_keybinds = &hsmk_load;

                            g_dialog_control[3] = 1;
                            page_count = calculate_dialog_metadata(g_dialog_text, _page_data);
                            g_dialog_control[2] = page_count;
                            g_dialog_page_data = _page_data;

                            hmrMenuCache(MENU_DIALOG);
                        }

                        break; //dar load
                    }
                    case 3: {
                        if (verify_Save(botao_selecionado_principal)) {
                            // g_dialog_text = "Save file not found.\nPress any key to continue.\0";
                            // dtext = (char*)malloc(40*2*sizeof(char));
                            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.not_found"));
                            // replace_substring(dtext, "{__NL}", "\n");

                            // g_dialog_text = dtext;
                            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 0].value;

                            int gsl = 0, gsw = 0;
                            get_multiline_stats(
                                g_dialog_text,
                                &gsl,
                                &gsw
                            );

                            g_ui_size[0] = gsl + 4;
                            g_ui_size[1] = gsw + 5;
                            g_dialog_control[0] = gsw + 3;
                            g_dialog_control[1] = gsl;

                            // g_ui_size[0] = 5;
                            // g_ui_size[1] = 40;
                            // g_dialog_control[0] = 40;
                            // g_dialog_control[1] = 2;
                            g_dialog_control[3] = 1;

                            char**** _page_data = malloc(sizeof(char****));
                            int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

                            g_dialog_control[2] = page_count;
                            g_dialog_page_data = _page_data;
                            displayMenu(MENU_DIALOG);
                        } else {
                            // g_dialog_text = "Are you sure you want to delete this save?\nTHIS ACTION IS NOT REVERSIBLE!\n(y/n)";
                            // dtext = (char*)malloc(50*3*sizeof(char));
                            // strcpy(dtext, get_localized_string(g_renderstate->language, "menu.saves.infos.dialogs.delete_confirm"));
                            // replace_substring(dtext, "{__NL}", "\n");

                            // g_dialog_text = dtext;
                            
                            g_dialog_text = cached_translations[2 + BOTOES_INFOS + BOTOES_PRINCIPAL + 4].value;

                            int gsl = 0, gsw = 0;
                            get_multiline_stats(
                                g_dialog_text,
                                &gsl,
                                &gsw
                            );

                            // debug_file(dbgOut, 0, "GSL: %d | GSW: %d\n", gsl, gsw);

                            g_ui_size[0] = gsl + 4;
                            g_ui_size[1] = gsw + 5;
                            g_dialog_control[0] = gsw + 3;
                            g_dialog_control[1] = gsl;

                            // debug_file(dbgOut, 0, "GUS_X: %d | GUS_Y: %d\n", g_ui_size[0], g_ui_size[1]);
                            // debug_file(dbgOut, 0, "GDC_0: %d | GDC_1: %d\n", g_dialog_control[0], g_dialog_control[1]);

                            // g_ui_size[0] = 6;
                            // g_ui_size[1] = 50;
                            // g_dialog_control[0] = 50;
                            // g_dialog_control[1] = 3;
                            g_dialog_control[3] = 1;
                            g_dialog_keybinds = &hsmk_delete;

                            char**** _page_data = malloc(sizeof(char****));
                            int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

                            g_dialog_control[2] = page_count;
                            g_dialog_page_data = _page_data;

                            // debug_file(dbgOut, 0, "GDPD: '%s'\n", *g_dialog_page_data[0][0]);

                            displayMenu(MENU_DIALOG);
                        }
                        break; //dar dielete
                    }
                }
                break;
            case 'Q':
            case 'q':
                closeMenu(MENU_SAVE_SLOT);
                save_selecionado = FALSE;
                break;
        }
    }
}

void cleanup_SaveMenu() {
    // effect_principal = 0, 
    botao_selecionado_principal = 0, 
    save_selecionado = FALSE, 
    botao_selecionado_infos = 0;
    free(dtext);
}
