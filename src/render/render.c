#include "render.h"

#define OFFSET_X 1
#define OFFSET_Y ALTURA_LOGO + 1


Renderstate g_renderstate;

Renderstate init_render() {
    debug_file(dbgOut, " - Allocating renderstate memory...\n");

    Renderstate rs = (Renderstate)malloc(sizeof(RENDERSTATE));

    rs->activeMenus = 0;
    for (int i = 0; i < MENU_STACK_MAX; i++) rs->menus[i] = NULL;

    // setlocale(LC_ALL, "");

    // Initialize window
    debug_file(dbgOut, " - Initializing window...\n");
    WINDOW *wnd = initscr();

    // Get window size
    debug_file(dbgOut, " - Initializing settings...\n");

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
    debug_file(dbgOut, " - Initializing colors...\n");

    start_color();

    init_color(GREY, 400, 400, 400); 
    init_color(DARK_DARK_GREY, 80, 80, 80);     
    init_color(DARK_GREY, 192, 192, 180); 
    init_color(BROWN, 624, 296, 0); 
    init_color(DARK_RED, 400, 0, 0); 
    init_color(DARK_GREEN, 0, 300,0);     
    init_color(LIGHT_GREY, 160, 160, 160);
    init_color(GREEN, 0, 700, 0);
    init_color(ORANGE, 830, 470, 0);
    init_color(LIGHT_ORANGE, 800, 500, 0);
    init_color(DARK_ORANGE, 760, 390, 0);
    init_color(LIGHTPLUS_GREY, 768, 768, 768);
    init_color(YELLOW_ORANGE, 870, 580, 0);
    init_color(DARKPLUS_GREY, 420, 420, 420);
    // PLAYER
	init_pair(WHITE_PLAYER, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW_PLAYER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_PLAYER, COLOR_BLUE, COLOR_BLACK);
    // DUNGEON
    init_pair(DUNGEON_WALLS, GREY, (short)(DARK_DARK_GREY | A_DIM)); 
    init_pair(DUNGEON_FLOOR, GREY, (short)(COLOR_BLACK | A_DIM));
    init_pair(DUNGEON_BLOOD, DARK_RED, (short)(COLOR_BLACK | A_DIM));     
    // ASYLUM
    init_pair(ASYLUM_WALLS, COLOR_BLACK, (short)(COLOR_WHITE | A_DIM)); 
    init_pair(ASYLUM_FLOOR, COLOR_WHITE, (short)(LIGHT_GREY | A_DIM)); 
    init_pair(ASYLUM_BLOOD, DARK_RED, (short)(LIGHT_GREY | A_DIM));    
    // SEWERS
    init_pair(SEWERS_BLOOD, COLOR_WHITE, (short)(DARK_RED | A_DIM));
    init_pair(SEWERS_FLOOR, BROWN, (short)(DARK_GREY | A_DIM)); 
    init_pair(SEWERS_WALLS, GREEN, (short)(DARK_GREEN | A_DIM));
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
    rs->wnd = wnd;

    g_renderstate = rs;

    debug_file(dbgOut, " - Loading localization files...\n");
    load_locales();

    return rs;
}

// Warning: DO NOT use the cycle here. The game cycle is controlled by the gameloop.
void render(Gamestate gs) {
    if(isInMenu() == 0)
        print_random_map(g_renderstate->nrows, g_renderstate->ncols - 2, find_map, OFFSET_Y, OFFSET_X); // map
    render_game(gs);
    print_light(g_renderstate->wnd, g_renderstate->nrows, g_renderstate->ncols-2, OFFSET_Y, OFFSET_X); // RTX_ON
    render_menu(gs);
    doupdate();
    // refresh();
}

#pragma region Menu functions
Menu getActiveMenu() {
    if (g_renderstate->activeMenus == MENU_STACK_MAX) return NULL;

    return g_renderstate->menus[g_renderstate->activeMenus - 1];
}

Menu displayMenu(MenuId menu) {
    if (g_renderstate->activeMenus == MENU_STACK_MAX) return NULL;
    if (isMenuDisplayed(menu)) return NULL;

    Menu nmenu = (Menu)malloc(sizeof(MENU));
    nmenu->wnd = NULL;
    nmenu->id = menu;
    nmenu->active = 1;

    g_renderstate->menus[g_renderstate->activeMenus++] = nmenu;
    return nmenu;
}

int closeMenu(MenuId menu) {
    if (g_renderstate->activeMenus == 0) return 0;
    if (!isMenuDisplayed(menu)) return 0;
        
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
    if (g_renderstate->activeMenus == 0) return;

    for (int i = 0; i < MENU_STACK_MAX; i++) {
        if (g_renderstate->menus[i]->id == menu) {
            for (int j = i; j < MENU_STACK_MAX - 1; j++) {
                if (g_renderstate->menus[j] != NULL) {
                    del_panel(g_renderstate->menus[j]->panel);
                    delwin(g_renderstate->menus[j]->wnd);

                    g_renderstate->menus[j] = NULL;
                    g_renderstate->activeMenus--;
                }
            }

            doupdate();

            return;
        }
    }
}
#pragma endregion

#pragma region Renderers
void render_game(Gamestate gs) {
    Coords playerCoords = gs->player->entity->coords;
    Coords projectileCoords = gs->projectile->entity->coords;

    // TEST
    // wattron(g_renderstate->wnd, COLOR_PAIR(YELLOW_BG));
    // for (int i = 0; i < g_gamestate->path_cell_count; i++) {
    //     mvwaddch(g_renderstate->wnd, g_gamestate->path_cells[i]->y, g_gamestate->path_cells[i]->x, '&');
    // }
    // wattron(g_renderstate->wnd, COLOR_PAIR(YELLOW_BG));

    // attron(COLOR_PAIR(GREEN_BG));
    // mvwaddch(g_renderstate->wnd, g_gamestate->pointA->y, g_gamestate->pointA->x, '#');
    // attroff(COLOR_PAIR(GREEN_BG));

    // attron(COLOR_PAIR(RED_BG));
    // mvwaddch(g_renderstate->wnd, g_gamestate->pointB->y, g_gamestate->pointB->x, '#');
    // attroff(COLOR_PAIR(RED_BG));

    move(g_renderstate->nrows - 1, 0);
	wattron(g_renderstate->wnd, COLOR_PAIR(BLUE_PLAYER));
	// printw("(%d, %d) %d %d | (%d, %d) (%d, %d) | %d | %d", 
    printw("(%d, %d) %d %d | %d %d", 
        playerCoords->x, 
        playerCoords->y, 
        g_renderstate->ncols, 
        g_renderstate->nrows,
        projectileCoords->x,
        projectileCoords->x
        // g_gamestate->pointA->x, g_gamestate->pointA->y,
        // g_gamestate->pointB->x, g_gamestate->pointB->y,
        // g_gamestate->path_cell_count,
        // g_gamestate->last_res
    );
	wattroff(g_renderstate->wnd, COLOR_PAIR(BLUE_PLAYER));

    if(g_gamestate->player->cheats == 1) {	
        wattron(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));
     }else {	
        wattron(g_renderstate->wnd, COLOR_PAIR(WHITE_PLAYER));
    }
	mvwaddch(g_renderstate->wnd, playerCoords->y + OFFSET_Y, playerCoords->x + OFFSET_X, '@');
    mvwaddch(g_renderstate->wnd, projectileCoords->y + OFFSET_Y, projectileCoords->x + OFFSET_X, 'T' | COLOR_PAIR(WHITE_PLAYER));
    // mvaddwstr(playerCoords->x, playerCoords->y, L"█");
	wattroff(g_renderstate->wnd, COLOR_PAIR(WHITE_PLAYER));
    wattroff(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));

    wattron(g_renderstate->wnd, COLOR_PAIR(ORANGE_LOGO));
    for (int i = 0; i < gs->mob_count; i++) {
        mvwaddch(
            g_renderstate->wnd, 
            gs->mobs[i]->entity->coords->y + OFFSET_Y, 
            gs->mobs[i]->entity->coords->x + OFFSET_X, 
            '$' | COLOR_PAIR(ORANGE_LOGO)
        );
    }
    wattroff(g_renderstate->wnd, COLOR_PAIR(ORANGE_LOGO));

    //print_light(g_renderstate->wnd, g_renderstate->nrows, g_renderstate->ncols);

	// wattron(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));
	// mvwaddch(g_renderstate->wnd, playerCoords->y - 1, playerCoords->x - 1, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y - 1, playerCoords->x + 0, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y - 1, playerCoords->x + 1, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y + 0, playerCoords->x - 1, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y + 0, playerCoords->x + 1, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y + 1, playerCoords->x - 1, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y + 1, playerCoords->x + 0, '.' | A_BOLD);
	// mvwaddch(g_renderstate->wnd, playerCoords->y + 1, playerCoords->x + 1, '.' | A_BOLD);
    // wattroff(g_renderstate->wnd, COLOR_PAIR(YELLOW_PLAYER));

	move(playerCoords->x, playerCoords->y);
    move(projectileCoords->x, projectileCoords->y);
}

void render_menu(Gamestate gs) {
    IGNORE_ARG(gs);
    for (int i = 0; i < g_renderstate->activeMenus; i++) {
        drawMenu(g_renderstate->menus[i]);
    }

    if (g_renderstate->activeMenus != 0) update_panels();
}
#pragma endregion
