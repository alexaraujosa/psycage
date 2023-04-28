#include "render.h"

Renderstate g_renderstate;

Renderstate init_render() {
    Renderstate rs = (Renderstate)malloc(sizeof(RENDERSTATE));

    // setlocale(LC_ALL, "");

    // Initialize window
    WINDOW *wnd = initscr();

    // Get window size
    int ncols, nrows;
    getmaxyx(wnd,nrows,ncols);
    rs->nrows = nrows;
    rs->ncols = ncols;

	start_color();

	cbreak();
	noecho();
    curs_set(0);
	nonl();
	intrflush(stdscr, false);
	keypad(stdscr, true);

    // COLORS
    init_color(GREY, 400, 400, 400); 
    init_color(DARK_DARK_GREY, 80, 80, 80);     
    init_color(DARK_GREY, 192, 192, 180); 
    init_color(BROWN, 624, 296, 0); 
    init_color(DARK_RED, 400, 0, 0); 
    init_color(DARK_GREEN, 0, 300,0);     
    init_color(LIGHT_GREY, 160, 160, 160);
    init_color(GREEN, 0, 700, 0);
    init_color(BLACK, 0 ,0 , 0);
    init_color(WHITE, 1000, 1000, 1000);
    // PLAYER
	init_pair(WHITE_PLAYER, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW_PLAYER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_PLAYER, COLOR_BLUE, COLOR_BLACK);
    // DUNGEON
    init_pair(DUNGEON_WALLS, GREY, DARK_DARK_GREY| A_DIM); 
    init_pair(DUNGEON_FLOOR, GREY, BLACK | A_DIM);
    init_pair(DUNGEON_BLOOD, DARK_RED, BLACK | A_DIM);     
    // ASYLUM
    init_pair(ASYLUM_WALLS, BLACK, WHITE | A_DIM); 
    init_pair(ASYLUM_FLOOR, WHITE, LIGHT_GREY | A_DIM); 
    init_pair(ASYLUM_BLOOD, DARK_RED, LIGHT_GREY | A_DIM);
    init_pair(NOTHING, BLACK, BLACK);    
    // SEWERS
    init_pair(SEWERS_BLOOD, WHITE, DARK_RED | A_DIM);
    init_pair(SEWERS_FLOOR, BROWN, DARK_GREY | A_DIM); 
    init_pair(SEWERS_WALLS, GREEN, DARK_GREEN | A_DIM);

    rs->wnd = wnd;

    rs->activeMenus = 0;
    for (int i = 0; i < MENU_STACK_MAX; i++) rs->menus[i] = NULL;

    g_renderstate = rs;
    return rs;
}

// Warning: DO NOT use the cycle here. The game cycle is controlled by the gameloop.
void render(Gamestate gs) {

    print_asylum(g_renderstate->nrows, g_renderstate->ncols); // map
    render_game(gs);
    render_menu(gs);
    doupdate();
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

    move(g_renderstate->nrows - 1, 0);
	attron(COLOR_PAIR(BLUE_PLAYER));
	printw("(%d, %d) %d %d", 
        playerCoords->x, 
        playerCoords->y, 
        g_renderstate->ncols, 
        g_renderstate->nrows
    );
	attroff(COLOR_PAIR(BLUE_PLAYER));

	attron(COLOR_PAIR(WHITE_PLAYER));
	mvaddch(playerCoords->x, playerCoords->y, '@' | COLOR_PAIR(WHITE_PLAYER));
    // mvaddwstr(playerCoords->x, playerCoords->y, L"█");
	attroff(COLOR_PAIR(WHITE_PLAYER));

	attron(COLOR_PAIR(YELLOW_PLAYER));
	mvaddch(playerCoords->x - 1, playerCoords->y - 1, '.' | A_BOLD);
	mvaddch(playerCoords->x - 1, playerCoords->y + 0, '.' | A_BOLD);
	mvaddch(playerCoords->x - 1, playerCoords->y + 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 0, playerCoords->y - 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 0, playerCoords->y + 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 1, playerCoords->y - 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 1, playerCoords->y + 0, '.' | A_BOLD);
	mvaddch(playerCoords->x + 1, playerCoords->y + 1, '.' | A_BOLD);
    // mvaddwstr(playerCoords->x - 1, playerCoords->y - 1, L"█");
	// mvaddwstr(playerCoords->x - 1, playerCoords->y + 0, L"█");
	// mvaddwstr(playerCoords->x - 1, playerCoords->y + 1, L"█");
	// mvaddwstr(playerCoords->x + 0, playerCoords->y - 1, L"█");
	// mvaddwstr(playerCoords->x + 0, playerCoords->y + 1, L"█");
	// mvaddwstr(playerCoords->x + 1, playerCoords->y - 1, L"█");
	// mvaddwstr(playerCoords->x + 1, playerCoords->y + 0, L"█");
	// mvaddwstr(playerCoords->x + 1, playerCoords->y + 1, L"█");
    attroff(COLOR_PAIR(YELLOW_PLAYER));

	move(playerCoords->x, playerCoords->y);
}

void render_menu(Gamestate gs) {
    for (int i = 0; i < g_renderstate->activeMenus; i++) {
        drawMenu(g_renderstate->menus[i]);
    }

    if (g_renderstate->activeMenus != 0) update_panels();
}
#pragma endregion
