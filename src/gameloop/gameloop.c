#include <stdio.h>
#include "gameloop.h"

Gamestate g_gamestate;
int find_map;

Gamestate init_gameloop() {
	Gamestate gs = (Gamestate)malloc(sizeof(GAMESTATE));
	Player player = defaultPlayer();
	Projectile projectile = defaultProjectile();
	gs->player = player;
	gs->projectile = projectile;

	gs->input_initialized = 0;

	gs->clock = 0;
	gs->block_clock = 1;

	gs->pointA = defaultCoords();
	gs->pointB = defaultCoords();
	gs->recalculate = 0;
	gs->path_cell_count = 0;
	gs->path_cells = NULL;

	g_gamestate = gs;

	find_map = create_random_map(g_renderstate->nrows, g_renderstate->ncols); // map

	//escolhe um dos 3s
	// create_dungeon();
    // create_asylum();
    // create_sewers();
	// create_ai_test_map();
	
	return gs;
}

void tick() {
	if (!g_gamestate->block_clock) g_gamestate->clock++;

	if (g_gamestate->recalculate) {
		int obstacles[2] = { 1, 3 };
		As_Node path = pathfind(
			g_renderstate->nrows, 
			g_renderstate->ncols, 
			map, 
			g_gamestate->pointA, 
			g_gamestate->pointB, 
			obstacles,
			2
		);

		g_gamestate->path_cell_count = pf_get_path_cell_count(path);
    	g_gamestate->path_cells = pf_reconstruct_path(path, g_gamestate->path_cell_count);
		__asm__("nop");

		g_gamestate->recalculate = 0;
	}

	handle_keybinds();

	if (isInMenu()) {
		Menu active_menu = getActiveMenu();
		if (active_menu == NULL) return;

		tick_menu(active_menu);
	} else {
		// RTX_ON
		calculate_visibility(g_gamestate->player->entity->coords->x,g_gamestate->player->entity->coords->y, map, g_renderstate->nrows, g_renderstate->ncols); 
	}
}

void handle_keybinds() {
	int key = getch();

	// switch(key) {
	// 	case 'z':
	// 	case 'Z':
	// 		randomize_atm_points();
	// 		break;
	// 	case 'q':
	// 	case 'Q':
	// 		endwin();
	// 		exit(0);
	// 		break;
    //     // default:
	// 	// 	// endwin();
	// 	// 	// printf("B\n");
    //     //     // exit(1);
    //         break;
    // }

	// return;

	// Fix initial duplicate keys
	if (!g_gamestate->input_initialized) {
		if (g_gamestate->block_clock) g_gamestate->block_clock = 0;
		if (g_gamestate->clock < 2) return;

		g_gamestate->block_clock = 1;
		g_gamestate->clock = 0;
		g_gamestate->input_initialized = 1;
	}

	if (isInMenu()) { menu_keybinds(key); return; }
	else { game_keybinds(key); return; }
}

void game_keybinds(int key) {
	mvaddch(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y, ' ');

	godmode_code_checker(key);

	switch(key) {
		case '1': {
			g_gamestate->pointA->x = g_gamestate->player->entity->coords->x;
			g_gamestate->pointA->y = g_gamestate->player->entity->coords->y;

			break;
		}
		case '2': {
			g_gamestate->pointB->x = g_gamestate->player->entity->coords->x;
			g_gamestate->pointB->y = g_gamestate->player->entity->coords->y;
			break;
		}
		case '3': {
			g_gamestate->recalculate = TRUE;
			break;
		}

		// Movement Controls
		case KEY_UP:
			move_player(0, -1);
			g_gamestate->player->last_direction = 0;  
			break;
		case KEY_DOWN:
			move_player(0, 1);
			g_gamestate->player->last_direction = 1;  
			break;
		case KEY_LEFT:
			move_player(-1, 0);
			g_gamestate->player->last_direction = 2;  
			break;
		case KEY_RIGHT:
			move_player(1, 0);
			g_gamestate->player->last_direction = 3;  
			break;
		case 'r':
		    if (g_gamestate->player->last_direction == 0) {
				g_gamestate->projectile->entity->coords->x == g_gamestate->player->entity->coords->x;
			g_gamestate->projectile->entity->coords->y == g_gamestate->player->entity->coords->y;
                move_projectile(0, -1); 
			}
			if (g_gamestate->player->last_direction == 1) {
				g_gamestate->projectile->entity->coords->x == g_gamestate->player->entity->coords->x;
			g_gamestate->projectile->entity->coords->y == g_gamestate->player->entity->coords->y;
				move_projectile(0, 1); 
			}
			if (g_gamestate->player->last_direction == 2) {
				g_gamestate->projectile->entity->coords->x == g_gamestate->player->entity->coords->x;
			g_gamestate->projectile->entity->coords->y == g_gamestate->player->entity->coords->y;
				move_projectile(-1, 0); 
			}
			if (g_gamestate->player->last_direction == 3) {
				g_gamestate->projectile->entity->coords->x == g_gamestate->player->entity->coords->x;
			g_gamestate->projectile->entity->coords->y == g_gamestate->player->entity->coords->y;
				move_projectile(1, 0); 
			}
			break;

		// Open Menu
		case '5': 
			displayMenu(MENU_MAIN_MENU);
			break;

		// Seppuku
		case 'q': 
			endwin(); 
			exit(0); 
			break;


		// case KEY_A1:
		// case '7': 
		// 	move_player(-1, -1); 
		// 	break;
		// case KEY_UP:
		// case '8': 
		// 	move_player(-1, +0); 
		// 	break;
		// case KEY_A3:
		// case '9': 
		// 	move_player(-1, +1);
		// 	break;
		// case KEY_LEFT:
		// case '4': 
		// 	move_player(+0, -1);
		// 	break;
		// case KEY_B2:
		// case '5': 
		// 	displayMenu(MENU_MAIN_MENU);
		// 	break;
		// case 'a':
		// case 'A': {
		// 	g_dialog_text = "A\nB\nLorem ipsum dolore sit amet. Some random fuckery here.\0";
		// 	g_ui_size[0] = 5;
		// 	g_ui_size[1] = 29;
		// 	g_dialog_control[0] = 29;
		// 	g_dialog_control[1] = 1;

		// 	char**** _page_data = malloc(sizeof(char****));
		// 	int page_count = calculate_dialog_metadata(g_dialog_text, _page_data);

		// 	g_dialog_control[2] = page_count;
		// 	g_dialog_page_data = _page_data;
		// 	displayMenu(MENU_DIALOG);
		// 	break;
		// }
		// case KEY_RIGHT:
		// case '6': 
		// 	move_player(+0, +1);
		// 	break;
		// case KEY_C1:
		// // case '1': 
		// 	move_player(+1, -1);
		// 	break;
		// case KEY_DOWN:
		// // case '2': 
		// 	move_player(+1, +0);
		// 	break;
		// case KEY_C3:
		// // case '3': 
		// 	move_player(+1, +1);
		// 	break;
		// case 'q': 
		// 	endwin(); 
		// 	exit(0); 
		// 	break;
	}
}

void menu_keybinds(int key) {
	if (!isInMenu()) return;

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

void move_projectile(int dx, int dy) {
	g_gamestate->projectile->entity->coords->x += dx;
	g_gamestate->projectile->entity->coords->y += dy;
}