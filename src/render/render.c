#include "render.h"

Renderstate g_renderstate;

Renderstate init_render() {
    Renderstate rs = (Renderstate)malloc(sizeof(RENDERSTATE));

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
	nonl();
	intrflush(stdscr, false);
	keypad(stdscr, true);

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);

    rs->wnd = wnd;

    rs->activeMenus = 0;
    for (int i = 0; i < MENU_STACK_MAX; i++) rs->menus[i] = NULL;

    g_renderstate = rs;
    return rs;
}

// Warning: DO NOT use the cycle here. The game cycle is controlled by the gameloop.
void render(Gamestate gs) {
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
	attron(COLOR_PAIR(COLOR_BLUE));
	printw("(%d, %d) %d %d", 
        playerCoords->x, 
        playerCoords->y, 
        g_renderstate->ncols, 
        g_renderstate->nrows
    );
	attroff(COLOR_PAIR(COLOR_BLUE));

	attron(COLOR_PAIR(COLOR_WHITE));
	mvaddch(playerCoords->x, playerCoords->y, '@' | A_BOLD);
	attroff(COLOR_PAIR(COLOR_WHITE));

	attron(COLOR_PAIR(COLOR_YELLOW));
	mvaddch(playerCoords->x - 1, playerCoords->y - 1, '.' | A_BOLD);
	mvaddch(playerCoords->x - 1, playerCoords->y + 0, '.' | A_BOLD);
	mvaddch(playerCoords->x - 1, playerCoords->y + 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 0, playerCoords->y - 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 0, playerCoords->y + 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 1, playerCoords->y - 1, '.' | A_BOLD);
	mvaddch(playerCoords->x + 1, playerCoords->y + 0, '.' | A_BOLD);
	mvaddch(playerCoords->x + 1, playerCoords->y + 1, '.' | A_BOLD);
    attroff(COLOR_PAIR(COLOR_YELLOW));

	move(playerCoords->x, playerCoords->y);
}

void render_menu(Gamestate gs) {
    for (int i = 0; i < g_renderstate->activeMenus; i++) {
        drawMenu(g_renderstate->menus[i]);
    }

    if (g_renderstate->activeMenus != 0) update_panels();
}
#pragma endregion
