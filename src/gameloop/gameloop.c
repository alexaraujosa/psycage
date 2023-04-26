#include <stdio.h>
#include "gameloop.h"

Gamestate g_gamestate;

Gamestate init_gameloop() {
	Gamestate gs = (Gamestate)malloc(sizeof(GAMESTATE));
	
	Player player = defaultPlayer();
	gs->player = player;

	g_gamestate = gs;
	return gs;
}

void tick() {
	handle_keybinds();

	if (isInMenu()) {
		Menu active_menu = getActiveMenu();
		if (active_menu == NULL) return;

		tick_menu(active_menu);
	} else {

	}
}

void handle_keybinds() {
	int key = getch();

	if (isInMenu()) { menu_keybinds(key); return; }
	{ game_keybinds(key); return; }
}

void game_keybinds(int key) {
	mvaddch(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y, ' ');

	switch(key) {
		case KEY_A1:
		case '7': 
			move_player(-1, -1); 
			break;
		case KEY_UP:
		case '8': 
			move_player(-1, +0); 
			break;
		case KEY_A3:
		case '9': 
			move_player(-1, +1);
			break;
		case KEY_LEFT:
		case '4': 
			move_player(+0, -1);
			break;
		case KEY_B2:
		case '5': 
			displayMenu(MENU_MAIN_MENU);
			break;
		case 'a':
		case 'A': {
			g_dialog_text = "A\nB\nLorem ipsum dolore sit amet. Some random fuckery here.\0";
			g_ui_size[0] = 5;
			g_ui_size[1] = 29;
			g_dialog_control[0] = 29;
			g_dialog_control[1] = 1;

			char**** _page_data = malloc(sizeof(char****));
			int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

			g_dialog_control[2] = page_count;
			g_dialog_page_data = _page_data;
			displayMenu(MENU_DIALOG);
			break;
		}
		case KEY_RIGHT:
		case '6': 
			move_player(+0, +1);
			break;
		case KEY_C1:
		case '1': 
			move_player(+1, -1);
			break;
		case KEY_DOWN:
		case '2': 
			move_player(+1, +0);
			break;
		case KEY_C3:
		case '3': 
			move_player(+1, +1);
			break;
		case 'q': 
			endwin(); 
			exit(0); 
			break;
	}
}

void menu_keybinds(int key) {
	switch (key) {
		case KEY_B2:
		case '5': 
			closeMenu(MENU_MAIN_MENU);
			break;
		default: {
			Menu active_menu = getActiveMenu();
			if (active_menu == NULL) break;
			
			handle_menu_keybinds(active_menu, key);
		}
	}
}

void move_player(int dx, int dy) {
	g_gamestate->player->entity->coords->x += dx;
	g_gamestate->player->entity->coords->y += dy;
}