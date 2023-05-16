#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "gameloop.h"

Gamestate g_gamestate;
int** map_footprint;
int find_map;

#define TICK_DURATION_MS 100
clock_t tickStart, tickEnd;
float tickDuration, taskDuration;

Gamestate init_gameloop() {
    debug_file(dbgOut, " - Allocating gameloop memory...\n");

	Gamestate gs = (Gamestate)malloc(sizeof(GAMESTATE));

    debug_file(dbgOut, " - Initializing player...\n");
	Player player = defaultPlayer();
	gs->player = player;

    debug_file(dbgOut, " - Initializing projectile...\n");
	Projectile projectile = defaultProjectile();
	gs->projectile = projectile;

    debug_file(dbgOut, " - Initializing clock-related variables...\n");
	gs->input_initialized = 0;
	gs->clock = 0;
	gs->block_clock = 1;

	// gs->pointA = defaultCoords();
	// gs->pointB = defaultCoords();
	// gs->recalculate = 0;
	// gs->path_cell_count = 0;
	// gs->path_cells = NULL;

    debug_file(dbgOut, " - Generating initial map...\n");
	find_map = create_random_map(g_renderstate->nrows - ALTURA_LOGO - 2, g_renderstate->ncols - 2, ALTURA_LOGO + 1, 1); // map

    debug_file(dbgOut, " -- Generating light map...\n");
	init_light_map(g_renderstate->nrows, g_renderstate->ncols);


    debug_file(dbgOut, " - Generating map footprint...\n");
	map_footprint = (int **)malloc((g_renderstate->nrows - ALTURA_LOGO - 2) * sizeof(int *));
    for (int i = 0; i < g_renderstate->nrows - ALTURA_LOGO - 2; i++) {
        map_footprint[i] = (int *)malloc(g_renderstate->ncols * sizeof(int));
    }

	for (int i = 0; i < g_renderstate->nrows - ALTURA_LOGO - 2; i++) {
		for (int j = 0; j < g_renderstate->ncols; j++) {
			map_footprint[i][j] = map[i][j];
		}
	}

	// gs->pointB = mob->entity->coords;

    debug_file(dbgOut, " - Generating mobs...\n");
	int mob_count = 3;
	Mob* mobs = (Mob*)malloc(sizeof(Mob) * mob_count);

	for (int i = 0; i < mob_count; i++) {
		Mob mob = defaultMob();

		addMobToMap(mob, map, g_renderstate->ncols, g_renderstate->nrows - ALTURA_LOGO - 2);

		mobs[i] = mob;
		map[mob->entity->coords->y][mob->entity->coords->x] = 5;
	}

	gs->mobs = mobs;
	gs->mob_count = mob_count;

	gs->last_res = -1;

	gs->paused = FALSE;

	//escolhe um dos 3s
	// create_dungeon();
    // create_asylum();
    // create_sewers();
	// create_ai_test_map();
	
	g_gamestate = gs;

    gs->player->radius = 5; // DELETE BEFORE SENDING THE PROJECT	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return gs;
}

void tick() {
	tickStart = clock();

	if (!g_gamestate->block_clock) g_gamestate->clock++;

	handle_keybinds();
	flushinp();

	if (isInMenu()) {
		Menu active_menu = getActiveMenu();
		if (active_menu == NULL) goto tick_end;

		tick_menu(active_menu);
	} else {
		for (int i = 0; i < g_gamestate->mob_count; i++) {
			attemptMoveMob(
				g_gamestate->player->entity->coords, 
				g_gamestate->mobs[i],
				map, 
				g_renderstate->ncols,
				g_renderstate->nrows
			);
		}

		// RTX_ON
		calculate_visibility(
			g_gamestate->player->entity->coords->x, 
			g_gamestate->player->entity->coords->y, 
			map, 
			g_renderstate->nrows, 
			g_renderstate->ncols
		); 
	}

	tick_end: {
		taskDuration = ((float)(clock() - tickStart)) / CLOCKS_PER_SEC * 1000;

		tickDuration = TICK_DURATION_MS - taskDuration;
		if (tickDuration > 0) {
            // Delay for the remaining time
            usleep((useconds_t)(tickDuration * 1000));

			// flushinp();
        }

		tickEnd = clock();

		// IF DEBUG NEEDED
		float actualTickDuration = ((float)(tickEnd - tickStart)) / CLOCKS_PER_SEC * 1000;
		// debug_file(dbgOut, "Tick duration: %.2f ms\n", actualTickDuration);

		if (tickDuration > TICK_DURATION_MS) 
			debug_file(dbgOut, "Tick took too long (%.2f / %d ms). Skipping.\n", actualTickDuration, TICK_DURATION_MS);
	}
}

void handle_keybinds() {
	int key = getch();

	if (key == ERR) return;

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
	// mvaddch(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y, ' ');

	godmode_code_checker(key);

	switch(key) {
		// case '1': {
		// 	g_gamestate->pointA->x = g_gamestate->player->entity->coords->x;
		// 	g_gamestate->pointA->y = g_gamestate->player->entity->coords->y;

		// 	break;
		// }
		// case '2': {
		// 	g_gamestate->pointB->x = g_gamestate->player->entity->coords->x;
		// 	g_gamestate->pointB->y = g_gamestate->player->entity->coords->y;
		// 	break;
		// }
		case '3': {
			// g_gamestate->recalculate = TRUE;
			g_gamestate->last_res = attemptMoveMob(
				g_gamestate->player->entity->coords, 
				g_gamestate->mobs[0],
				map, 
				g_renderstate->ncols,
				g_renderstate->nrows
			);
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