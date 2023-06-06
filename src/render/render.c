#include "render.h"


Renderstate g_renderstate;

Renderstate init_render() {
    debug_file(dbgOut, 0, " - Allocating renderstate memory...\n");

    Renderstate rs = (Renderstate)malloc(sizeof(RENDERSTATE));

    rs->activeMenus = 0;
    for (int i = 0; i < MENU_STACK_MAX; i++) rs->menus[i] = NULL;

    // setlocale(LC_ALL, "");

    // Initialize window
    debug_file(dbgOut, 0, " - Initializing window...\n");
    WINDOW *wnd = initscr();

    // Get window size
    debug_file(dbgOut, 0, " - Initializing settings...\n");

    int ncols, nrows;
    getmaxyx(wnd,nrows,ncols);
    rs->nrows = nrows;
    rs->ncols = ncols;
    rs->language = en_US;

	cbreak();
	noecho();
    curs_set(0);
	nonl();
    nodelay(stdscr, TRUE);
    //timeout(100);
	intrflush(stdscr, false);
	keypad(stdscr, true);
    //curs_set(0);

    
    // COLORS
    debug_file(dbgOut, 0, " - Initializing colors...\n");

    start_color();

    init_color(GREY, 400, 400, 400); 
    init_color(DARK_DARK_GREY, 80, 80, 80);     
    init_color(DARK_GREY, 192, 192, 180); 
    init_color(BROWN, 624, 296, 0); 
    init_color(DARK_BROWN, 600, 270, 0);
    init_color(DARK_RED, 400, 0, 0); 
    init_color(DARK_DARK_RED, 200, 0, 100);
    init_color(DARK_GREEN, 0, 300,0);     
    init_color(DARK_DARK_GREEN, 0, 250, 0);
    init_color(LIGHT_GREY, 160, 160, 160);
    init_color(GREEN, 0, 700, 0);
    init_color(ORANGE, 830, 470, 0);
    init_color(LIGHT_ORANGE, 800, 500, 0);
    init_color(DARK_ORANGE, 760, 390, 0);
    init_color(LIGHTPLUS_GREY, 768, 768, 768);
    init_color(YELLOW_ORANGE, 870, 580, 0);
    init_color(DARKPLUS_GREY, 420, 420, 420);
    init_color(PURPLE, 592, 0, 844);
    //DOORS
    init_pair(DOOR, COLOR_YELLOW, COLOR_BLUE);
    // PLAYER
	init_pair(WHITE_PLAYER, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW_PLAYER, COLOR_YELLOW, COLOR_BLACK);
    // init_pair(BLUE_PLAYER, COLOR_BLUE, COLOR_BLACK);
    // init_pair(BLUE_PLAYER, (short)(COLOR_BLUE | A_BOLD), COLOR_CYAN);
    init_pair(BLUE_PLAYER, (short)(COLOR_BLACK | A_BOLD), COLOR_BLUE);

    // DUNGEON
    init_pair(DUNGEON_WALLS, GREY, (short)(DARK_DARK_GREY | A_DIM)); 
    init_pair(DUNGEON_FLOOR, GREY, (short)(COLOR_BLACK | A_DIM));
    init_pair(DUNGEON_BLOOD, DARK_RED, (short)(COLOR_BLACK | A_DIM));     
    init_pair(DUNGEON_WALLS_VISITED, DARK_GREY, (short)(DARK_DARK_GREY | A_DIM)); 
    init_pair(DUNGEON_FLOOR_VISITED, DARK_GREY, (short)(COLOR_BLACK | A_DIM));
    init_pair(DUNGEON_BLOOD_VISITED, DARK_DARK_RED, (short)(COLOR_BLACK | A_DIM));     
    // ASYLUM
    init_pair(ASYLUM_WALLS, COLOR_BLACK, (short)(COLOR_WHITE | A_DIM)); 
    init_pair(ASYLUM_FLOOR, COLOR_WHITE, (short)(LIGHT_GREY | A_DIM)); 
    init_pair(ASYLUM_BLOOD, DARK_RED, (short)(LIGHT_GREY | A_DIM));    
    init_pair(ASYLUM_WALLS_VISITED, DARK_GREY, (short)(GREY | A_DIM)); 
    init_pair(ASYLUM_FLOOR_VISITED, GREY, (short)(DARK_GREY | A_DIM)); 
    init_pair(ASYLUM_BLOOD_VISITED, DARK_DARK_RED, (short)(DARK_GREY | A_DIM));   
    // SEWERS
    init_pair(SEWERS_BLOOD, COLOR_WHITE, (short)(DARK_RED | A_DIM));
    init_pair(SEWERS_FLOOR, BROWN, (short)(DARK_GREY | A_DIM)); 
    init_pair(SEWERS_WALLS, GREEN, (short)(DARK_GREEN | A_DIM));
    init_pair(SEWERS_BLOOD_VISITED, COLOR_WHITE, (short)(DARK_DARK_RED | A_DIM));
    init_pair(SEWERS_FLOOR_VISITED, DARK_BROWN, (short)(DARK_DARK_GREY | A_DIM)); 
    init_pair(SEWERS_WALLS_VISITED, DARK_GREEN, (short)(DARK_DARK_GREEN | A_DIM));
    // MENUS
    init_pair(ORANGE_LOGO, ORANGE, 0);
    init_pair(LIGHT_ORANGE_LOGO, LIGHT_ORANGE, 0);
    init_pair(DARK_ORANGE_LOGO, DARK_ORANGE, 0);
    init_pair(LIGHTPLUS_GREY_LOGO, LIGHTPLUS_GREY, 0);
    init_pair(YELLOW_ORANGE_LOGO, YELLOW_ORANGE, 0);
    init_pair(DARKPLUS_GREY_LOGO, DARKPLUS_GREY, 0);

    init_pair(RED_BG, COLOR_WHITE, COLOR_RED);
    init_pair(GREEN_BG, COLOR_WHITE, COLOR_GREEN);
    init_pair(YELLOW_BG, COLOR_WHITE, COLOR_YELLOW);
    init_pair(TRAP, GREEN, DARK_GREEN);
    init_pair(TRAP_VISITED, DARK_GREEN, DARK_DARK_GREEN);
    init_pair(MOLOTOV, ORANGE, DARK_RED);
    init_pair(MOLOTOV_VISITED, DARK_ORANGE, DARK_DARK_RED);
    init_pair(POTION, COLOR_WHITE, PURPLE);

    init_pair(MATRIX_BG, GREEN, DARK_GREEN);
    init_pair(MATRIX_FG, COLOR_BLACK, COLOR_BLACK);
    init_pair(SANITY_FULL, (short)(DARK_RED | A_BOLD), COLOR_RED);
    init_pair(SANITY_EMPTY, (short)(COLOR_RED | A_BOLD), DARK_RED);
    init_pair(SANITY_MARKER, (short)(COLOR_RED | A_BOLD), COLOR_BLACK);

    init_pair(EMPTY, COLOR_BLACK, COLOR_BLACK);

    rs->wnd = wnd;

    g_renderstate = rs;

    debug_file(dbgOut, 0, " - Loading localization files...\n");
    load_locales();

    debug_file(dbgOut, 0, " - Loading main menu...\n");
    displayMenu(MENU_MAIN_MENU);

    return rs;
}

// Warning: DO NOT use the cycle here. The game cycle is controlled by the gameloop.
void render(Gamestate gs) {
    if (!g_gamestate->valid_state) return;
    
    if (map != NULL && map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] == 4){
        print_loading_screen(g_renderstate->wnd, g_renderstate->nrows, g_renderstate->ncols);
    }
    else{
        if(isInMenu() == 0){
            print_random_map(ALTURA_JOGO, LARGURA_JOGO, find_map, OFFSET_Y, OFFSET_X); // map
            render_game(gs);
            print_light(g_renderstate->wnd, ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X);// RTX_ON
            render_foreground();
        } 

        render_menu(gs);
    }
    refresh();
}

#pragma region Menu functions
Menu getActiveMenu() {
    if (g_renderstate->activeMenus == MENU_STACK_MAX) return NULL;

    return g_renderstate->menus[g_renderstate->activeMenus - 1];
}

Menu displayMenu(MenuId menu) {
    debug_file(dbgOut, 1, "Attempting to change menu to %s.\n", stringify_menu_id(menu));

    if (g_renderstate->activeMenus == MENU_STACK_MAX) {
        debug_file(dbgOut, 3, "- Maximum menu stack reached. Ignoring incoming request.\n");
        return NULL;
    }
    if (isMenuDisplayed(menu)) {
        debug_file(dbgOut, 3, "- Menu already on display. Ignoring incoming request.\n");
        return NULL;
    }

    debug_file(dbgOut, 3, "- Accepting incoming request.\n");

    // Menu nmenu = (Menu)malloc(sizeof(MENU));
    // nmenu->wnd = NULL;
    // nmenu->id = menu;
    // nmenu->active = 1;

    Menu nmenu = getMenuCacheOrCreate(menu);
    if (nmenu == NULL) {
        debug_file(dbgOut, 1, "- Unable to create menu. Exiting.");
        EXIT = TRUE;
        return;
    }

    g_renderstate->menus[g_renderstate->activeMenus++] = nmenu;
    return nmenu;
}

int closeMenu(MenuId menu) {
    debug_file(dbgOut, 1, "Attempting to close menu %s.\n", stringify_menu_id(menu));
    if (g_renderstate->activeMenus == 0) {
        debug_file(dbgOut, 1, "- No menu displayed. Skipping.\n");
        return 0;
    }
    if (!isMenuDisplayed(menu)) {
        debug_file(dbgOut, 1, "- Menu not displayed. Skipping.\n");
        return 0;
    }

    debug_file(dbgOut, 1, "- Proceeding.\n");
        
    _removeMenu(menu);
    return 0;
}

int isMenuDisplayed(MenuId menu) {
    if (g_renderstate->activeMenus == 0) return 0;
    for (int i = 0; i < g_renderstate->activeMenus; i++) {
        if (g_renderstate->menus[i]->id == menu) return 1;
    }

    return 0;
}

int isInMenu() {
    return !!g_renderstate->activeMenus;
}

void _removeMenu(MenuId menu) {
    debug_file(dbgOut, 1, "- Removing menu hierarchy for %s.\n", stringify_menu_id(menu));
    if (g_renderstate->activeMenus == 0) return;

    for (int i = 0; i < MENU_STACK_MAX; i++) {
        if (g_renderstate->menus[i]->id == menu) {
            for (int j = i; j < MENU_STACK_MAX - 1; j++) {
                if (g_renderstate->menus[j] != NULL) {
                    debug_file(dbgOut, 1, "-- Cleaning menu %s.\n", stringify_menu_id(g_renderstate->menus[j]->id));
                    cleanup_menu(g_renderstate->menus[j]);

                    if (g_renderstate->menus[j]->valid == 0) {
                        debug_file(
                            dbgOut, 1, "-- Menu %s is marked for destruction. Destroying.\n", 
                            stringify_menu_id(g_renderstate->menus[j]->id)
                        );

                        // del_panel(g_renderstate->menus[j]->panel);
                        // delwin(g_renderstate->menus[j]->wnd);
                        // free(g_renderstate->menus[j]);

                        deleteMenuCache(g_renderstate->menus[j]->id);

                        debug_file(dbgOut, 1, "-- Successfully destroyed menu.\n");
                    } else {
                        // debug_file(dbgOut, 1, "-- Clearing menu %s.\n", stringify_menu_id(g_renderstate->menus[j]->id));
                        // cleanup_menu(g_renderstate->menus[j]);

                        // del_panel(g_renderstate->menus[j]->panel);
                        // delwin(g_renderstate->menus[j]->wnd);
                        wclear(g_renderstate->menus[j]->wnd);
                        hide_panel(g_renderstate->menus[j]->panel);
                        g_renderstate->menus[j]->active = 0;
                    }

                    g_renderstate->menus[j] = NULL;
                    g_renderstate->activeMenus--;

                    debug_file(dbgOut, 1, "-- Successfully cleaned up menu.\n");
                }
            }

            debug_file(dbgOut, 1, "-- Menus cleared.\n");

            update_panels();
            doupdate();

            return;
        }
    }
}
#pragma endregion

#pragma region Renderers
#include "util/ncurses.h" // THIS HAS TO BE HERE, OTHERWISE ALL HELL BREAKS LOOSE

void render_sanity_bar(WINDOW* wnd, int startX, int startY, int percentage) {
    // const int barWidth = 20;
    // int filledWidth = percentage * barWidth / 100;
    // int emptyWidth = barWidth - filledWidth;

    const int barWidth = 20;
    int filledWidth, emptyWidth;
    percentage = (percentage < -100) ? -100 : ((percentage > 100) ? 100 : percentage);

    if (percentage < 0) {
        // filledWidth = 0;
        // emptyWidth = (100 + percentage) * barWidth / 100;
        filledWidth = (100 + percentage) * barWidth / 100;
        emptyWidth = barWidth - filledWidth;
    } else {
        filledWidth = percentage * barWidth / 100;
        emptyWidth = barWidth - filledWidth;
    }

    wmove(wnd, startY, startX);

    wattron(wnd, COLOR_PAIR(SANITY_EMPTY));
    rectangle(wnd, startY - 1, 0, startY + 1, 27);
    // mvwprintw(wnd, startY, startX + 1, "%3d%%", percentage);
    if (percentage < 0) {
        mvwprintw(wnd, startY, startX + 1, "%3.3d%%", percentage);
    } else {
        mvwprintw(wnd, startY, startX + 1, " ");
        mvwprintw(wnd, startY, startX + 2, "%3.3d%%", percentage);
    }
    waddch(wnd, ' ');
    waddch(wnd, ' ');
    wattroff(wnd, COLOR_PAIR(SANITY_EMPTY));

    refresh();

    wmove(wnd, startY, startX + 7);
    if (percentage < 0) wattron(wnd, COLOR_PAIR(SANITY_EMPTY));
    else wattron(wnd, COLOR_PAIR(SANITY_FULL));
    for (int i = 0; i < filledWidth; i++) {
        waddch(wnd, '-');
    }
    if (percentage < 0) wattroff(wnd, COLOR_PAIR(SANITY_EMPTY));
    else wattroff(wnd, COLOR_PAIR(SANITY_FULL));

    if (percentage < 0) wattron(wnd, COLOR_PAIR(SANITY_FULL));
    else wattron(wnd, COLOR_PAIR(SANITY_EMPTY));
    for (int i = 0; i < emptyWidth; i++) {
        waddch(wnd, '_');
    }
    if (percentage < 0) wattroff(wnd, COLOR_PAIR(SANITY_FULL));
    else wattron(wnd, COLOR_PAIR(SANITY_EMPTY));

    // waddch(wnd, '|');
    refresh();
}

void render_foreground() {
    Coords playerCoords = g_gamestate->player->entity->coords;

    render_sanity_bar(g_renderstate->wnd, 0, g_renderstate->nrows - 3, g_gamestate->player->sanity);

	wattron(g_renderstate->wnd, COLOR_PAIR(BLUE_PLAYER));
    move(g_renderstate->nrows - 1, 0);
    printw("                            ");

    move(g_renderstate->nrows - 1, 0);
    printw(" X: %d Y: %d | %d", 
        playerCoords->x, 
        playerCoords->y,
        g_gamestate->player->candle_fuel
    );

	wattroff(g_renderstate->wnd, COLOR_PAIR(BLUE_PLAYER));
}

void render_game(Gamestate gs) {
    Coords playerCoords = gs->player->entity->coords;
    Coords projectileCoords = gs->projectiles[0]->entity->coords;

    // move(g_renderstate->nrows - 1, 0);
	// wattron(g_renderstate->wnd, COLOR_PAIR(BLUE_PLAYER));
	// // printw("(%d, %d) %d %d | (%d, %d) (%d, %d) | %d | %d", 
    // printw("(%d, %d) %d %d | %d %d", 
    //     playerCoords->x, 
    //     playerCoords->y, 
    //     g_renderstate->ncols, 
    //     g_renderstate->nrows,
    //     projectileCoords->x,
    //     projectileCoords->x
    //     // g_gamestate->pointA->x, g_gamestate->pointA->y,
    //     // g_gamestate->pointB->x, g_gamestate->pointB->y,
    //     // g_gamestate->path_cell_count,
    //     // g_gamestate->last_res
    // );
	// wattroff(g_renderstate->wnd, COLOR_PAIR(BLUE_PLAYER));

    if(g_gamestate->player->cheats->godmode == 1) {	
        wattron(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));
    } else {	
        wattron(g_renderstate->wnd, COLOR_PAIR(WHITE_PLAYER));
    }

    
	
    if(visible[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x] == 1 && 
       g_gamestate->projectiles[1]->entity->coords->y != 0 &&
       g_gamestate->projectiles[1]->entity->coords->x != 0
    )
        mvwaddch(g_renderstate->wnd, g_gamestate->projectiles[1]->entity->coords->y + OFFSET_Y, g_gamestate->projectiles[1]->entity->coords->x + OFFSET_X, 'T' | COLOR_PAIR(WHITE_PLAYER));

    if(visible[g_gamestate->projectiles[2]->entity->coords->y][g_gamestate->projectiles[2]->entity->coords->x] == 1 &&
       g_gamestate->projectiles[2]->entity->coords->y != 0 &&
       g_gamestate->projectiles[2]->entity->coords->x != 0
    )
        mvwaddch(g_renderstate->wnd, g_gamestate->projectiles[2]->entity->coords->y + OFFSET_Y, g_gamestate->projectiles[2]->entity->coords->x + OFFSET_X, 'X' | COLOR_PAIR(WHITE_PLAYER));





    for(int y = 0 ; y < ALTURA_JOGO ; y++){
        for(int x = 0 ; x < LARGURA_JOGO ; x++){
            
            if(map[y][x] == 8)
                mvaddch(y + OFFSET_Y, x + OFFSET_X, 'T' | COLOR_PAIR(TRAP));
            if(map[y][x] == 8 && visible[y][x] == 2 )
                mvaddch(y + OFFSET_Y, x + OFFSET_X, 'T' | COLOR_PAIR(TRAP_VISITED));

            if(map[y][x] == 9)  
                mvaddch(y + OFFSET_Y, x + OFFSET_X, 'Z' | COLOR_PAIR(RED_BG));

        }
    }



        
    if(visible[projectileCoords->y][projectileCoords->x] == 1 && projectileCoords->y != 0 && projectileCoords->x != 0)
        mvwaddch(g_renderstate->wnd, projectileCoords->y + OFFSET_Y, projectileCoords->x + OFFSET_X, 'O' | COLOR_PAIR(WHITE_PLAYER));



    if(g_gamestate->player->cheats->godmode == 1)	
        wattron(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));
    else	
        wattron(g_renderstate->wnd, COLOR_PAIR(WHITE_PLAYER));
    
	mvwaddch(g_renderstate->wnd, playerCoords->y + OFFSET_Y, playerCoords->x + OFFSET_X, '@');

	wattroff(g_renderstate->wnd, COLOR_PAIR(WHITE_PLAYER));
    wattroff(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));



    wattron(g_renderstate->wnd, COLOR_PAIR(ORANGE_LOGO));

    for (int i = 0; i < gs->mob_count; i++) {
        if(gs->mobs[i]->entity->health == 0)
        wattron(g_renderstate->wnd, COLOR_PAIR(RED_BG));
        if(visible[gs->mobs[i]->entity->coords->y][gs->mobs[i]->entity->coords->x] == 1)
            mvwaddch(
                g_renderstate->wnd, 
                gs->mobs[i]->entity->coords->y + OFFSET_Y, 
                gs->mobs[i]->entity->coords->x + OFFSET_X, 
                '$'
            );
    }
wattroff(g_renderstate->wnd, COLOR_PAIR(RED_BG));
    for (int i = 0; i < gs->chest_count; i++) {
        if(visible[gs->chests[i]->entity->coords->y][gs->chests[i]->entity->coords->x] == 1)
            mvwaddch(
                g_renderstate->wnd, 
                gs->chests[i]->entity->coords->y + OFFSET_Y, 
                gs->chests[i]->entity->coords->x + OFFSET_X, 
                'M'
            );
    }

    wattroff(g_renderstate->wnd, COLOR_PAIR(ORANGE_LOGO));



	move(playerCoords->x, playerCoords->y);
    move(projectileCoords->x, projectileCoords->y);

    doupdate();
}

void render_menu(Gamestate gs) {
    IGNORE_ARG(gs);
    
    debug_file(dbgOut, 3, "New menu render cycle.\n");
    for (int i = 0; i < g_renderstate->activeMenus; i++) {
        debug_file(dbgOut, 3, "- Rendering menu #%d.\n", g_renderstate->menus[i]->id);
        if (g_renderstate->menus[i]->active == 0) {
            show_panel(g_renderstate->menus[i]->panel);
            g_renderstate->menus[i]->active = 1;
        }

        drawMenu(g_renderstate->menus[i]);
    }

    if (g_renderstate->activeMenus != 0) update_panels();
}
#pragma endregion
