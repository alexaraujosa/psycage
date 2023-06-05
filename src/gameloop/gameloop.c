#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "gameloop.h"

extern int EXIT;
int keybinds[] = {
		KEY_UP,		
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		114,		// 'r'
		112,		// 'p'
		92			// '\\'
};

int ALTURA_JOGO;
int LARGURA_JOGO;

Gamestate g_gamestate;
int** map_footprint;
int find_map;

#define TICK_DURATION_MS 100
clock_t tickStart, tickEnd;
float tickDuration, taskDuration;

Gamestate init_gameloop() {
	debug_file(dbgOut, 0, " - Generating dimensions...\n");
	ALTURA_JOGO = g_renderstate->nrows - ALTURA_LOGO - 2;
	LARGURA_JOGO = g_renderstate->ncols - 2;

    debug_file(dbgOut, 0, " - Allocating gameloop memory...\n");

	Gamestate gs = (Gamestate)malloc(sizeof(GAMESTATE));

    debug_file(dbgOut, 0, " - Initializing player...\n");
	Player player = defaultPlayer();
	gs->player = player;

    debug_file(dbgOut, 0, " - Initializing projectiles...\n");
    int projectile_count = 2;
    Projectile* projectiles = (Projectile*)malloc(sizeof(Projectile) * projectile_count);
    for(int i = 0 ; i < projectile_count ; i++) {
            Projectile projectile = defaultProjectile();
            projectiles[i] = projectile;
    }
    gs->projectiles = projectiles;
    gs->projectile_count = projectile_count;

    debug_file(dbgOut, 0, " - Initializing clock-related variables...\n");
	gs->input_initialized = 1;
	gs->clock = 0;
	gs->block_clock = 1;

	// gs->pointA = defaultCoords();
	// gs->pointB = defaultCoords();
	// gs->recalculate = 0;
	// gs->path_cell_count = 0;
	// gs->path_cells = NULL;

    debug_file(dbgOut, 0, " -- Generating light map...\n");
	init_light_map(ALTURA_JOGO, LARGURA_JOGO);

    debug_file(dbgOut, 0, " - Generating initial map...\n");
	find_map = create_random_map(ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X); // map
	// (g_renderstate->nrows - ALTURA_LOGO - 2) + (OFFSET_Y)
	// g_renderstate->nrows - ALTURA_LOGO - 2 + OFFSET_Y
	// g_renderstate->nrows - ALTURA_LOGO + ALTURA_LOGO - 2 + 1
	// g_renderstate->nrows - 0 - 1
	// g_renderstate->nrows - 1, LARGURA_JOGO




    debug_file(dbgOut, 0, " - Generating map footprint...\n");
	map_footprint = (int **)malloc((ALTURA_JOGO) * sizeof(int *));
    for (int i = 0; i < ALTURA_JOGO; i++) {
        map_footprint[i] = (int *)malloc((LARGURA_JOGO) * sizeof(int));
    }

	for (int i = 0; i < ALTURA_JOGO; i++) {
		for (int j = 0; j < LARGURA_JOGO; j++) {
			map_footprint[i][j] = map[i][j];
		}
	}

	// gs->pointB = mob->entity->coords;

    debug_file(dbgOut, 0, " - Generating mobs...\n");
	int mob_count = 3;
	Mob* mobs = (Mob*)malloc(sizeof(Mob) * mob_count);

	for (int i = 0; i < mob_count; i++) {
		Mob mob = defaultMob();
		addRandomItemToMob(mob);

		addMobToMap(mob, map, LARGURA_JOGO, ALTURA_JOGO);

		mobs[i] = mob;
		map[mob->entity->coords->y][mob->entity->coords->x] = 5;
	}

	gs->mobs = mobs;
	gs->mob_count = mob_count;

	debug_file(dbgOut, 0 , " - Generating chests...\n");
    int chest_count = 1;
	Chest* chests = (Chest*)malloc(sizeof(Chest) * chest_count);

	for (int i = 0; i < chest_count; i++) {
	    Chest chest = defaultChest();

	    addChestToMap(chest, map, LARGURA_JOGO, ALTURA_JOGO);

	    chests[i] = chest;
	    map[chest->entity->coords->y][chest->entity->coords->x] = 5;
	}

	gs->chests = chests;
	gs->chest_count = chest_count;

	gs->last_res = -1;

	gs->paused = FALSE;

	g_gamestate = gs;

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
		if (is_player_dead(g_gamestate->player)) {
			displayMenu(MENU_DEAD);
			goto tick_end;
		}

		for (int i = 0; i < g_gamestate->mob_count; i++) {
			attemptMoveMob(
				g_gamestate->player->entity->coords, 
				g_gamestate->mobs[i],
				map, 
				LARGURA_JOGO,
				ALTURA_JOGO
			);
		}

		if(g_gamestate->projectiles[0]->entity->coords->x != 0 && g_gamestate->projectiles[0]->entity->coords->y != 0)
			move_projectile(g_gamestate->projectiles[0]->dx, g_gamestate->projectiles[0]->dy);	

		if(g_gamestate->projectiles[1]->entity->coords->x != 0 && g_gamestate->projectiles[1]->entity->coords->y != 0)
            moveSmoke(g_gamestate->projectiles[1]->dx, g_gamestate->projectiles[1]->dy); 	

		// RTX_ON
		calculate_visibility(
			g_gamestate->player->entity->coords->x, 
			g_gamestate->player->entity->coords->y, 
			map, 
			ALTURA_JOGO, 
			LARGURA_JOGO
		); 




		if(g_gamestate->mob_count == 0) continue_game(ALTURA_JOGO, LARGURA_JOGO);
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
		// debug_file(dbgOut, 0, "Tick duration: %.2f ms\n", actualTickDuration);

		if (actualTickDuration > TICK_DURATION_MS) 
			debug_file(dbgOut, 0, "Tick took too long (%.2f / %d ms). Skipping.\n", actualTickDuration, TICK_DURATION_MS);
	}
}

void handle_keybinds() {
	int key = getch();

	if (key == ERR) return;

	debug_file(dbgOut, 1, "Recieved key event: %d\n", key);

	// Fix initial duplicate keys
	// if (!g_gamestate->input_initialized) {
	// 	if (g_gamestate->block_clock) g_gamestate->block_clock = 0;
	// 	if (g_gamestate->clock < 2) return;

	// 	g_gamestate->block_clock = 1;
	// 	g_gamestate->clock = 0;
	// 	g_gamestate->input_initialized = 1;
	// }

	// switch (key) {
	// 	case '\\':
	// 		if (isMenuDisplayed(MENU_CONSOLE)) { menu_keybinds(key); return; }
	// 		else { displayMenu(MENU_CONSOLE); return; }
	// 		break;
	// 	default:
	// 		if (isInMenu()) { menu_keybinds(key); return; }
	// 		else { game_keybinds(key); return; }
	// 		break;
	// }

	if (isInMenu()) { menu_keybinds(key); return; }
	else { game_keybinds(key); return; }
}

void game_keybinds(int key) {
	// mvaddch(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y, ' ');

	godmode_code_checker(key);
	vision_code_checker(key);

	if(key == 'z')
		continue_game();

    if(key == 'j')
            if(g_gamestate->projectiles[1]->entity->coords->x != 0 && g_gamestate->projectiles[1]->entity->coords->y != 0)        // se o player nao tiver colocado a smoke
                    removeSmoke();                                                                        // vai dar crash, ja que ao repor o map com o map_footprint vai ler map_footprint[-3][0], o que nao existe
            // Movement Controls

    // Movement Controls
    if(key == 'i') {
            if(g_gamestate->projectiles[1]->entity->coords->x == 0 && g_gamestate->projectiles[1]->entity->coords->y == 0){
                g_gamestate->projectiles[1]->entity->coords->x = g_gamestate->player->entity->coords->x;
                g_gamestate->projectiles[1]->entity->coords->y = g_gamestate->player->entity->coords->y;
            }
            if (g_gamestate->player->last_direction == 0) {
                    g_gamestate->projectiles[1]->dx = 0;
                    g_gamestate->projectiles[1]->dy = -1;
            }
            if (g_gamestate->player->last_direction == 1) {
                    g_gamestate->projectiles[1]->dx = 0;
                    g_gamestate->projectiles[1]->dy = 1;
            }
            if (g_gamestate->player->last_direction == 2) {
                    g_gamestate->projectiles[1]->dx = -1;
                    g_gamestate->projectiles[1]->dy = 0;
            }
               if (g_gamestate->player->last_direction == 3) {
                    g_gamestate->projectiles[1]->dx = 1;
                    g_gamestate->projectiles[1]->dy = 0;
            }
    }

		// Movement Controls
	if(key == keybinds[0]) {

		if(is_passable(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y-1)){
			move_player(0, -1);
			g_gamestate->player->last_direction = 0;  
			smokeChecker();
		}

	} else if (key == keybinds[1]) {	

		if(is_passable(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y+1)){
			move_player(0, 1);
			g_gamestate->player->last_direction = 1;  
			smokeChecker();
		}

	} else if (key == keybinds[2]) {

			if(is_passable(g_gamestate->player->entity->coords->x-1, g_gamestate->player->entity->coords->y)){
				move_player(-1, 0);
				g_gamestate->player->last_direction = 2;
				smokeChecker();
			}  
	
	} else if (key == keybinds[3]) {

			if(is_passable(g_gamestate->player->entity->coords->x+1, g_gamestate->player->entity->coords->y)){
				move_player(1, 0);
				g_gamestate->player->last_direction = 3;
				smokeChecker();
			}

	} else if (key == keybinds[4] || key == toupper(keybinds[4]) ) {

			g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->player->entity->coords->x;
			g_gamestate->projectiles[0]->entity->coords->y = g_gamestate->player->entity->coords->y;

		    if (g_gamestate->player->last_direction == 0) {
				g_gamestate->projectiles[0]->dx = 0;
				g_gamestate->projectiles[0]->dy = -1;
			}
			if (g_gamestate->player->last_direction == 1) {
				g_gamestate->projectiles[0]->dx = 0;
				g_gamestate->projectiles[0]->dy = 1;
			}
			if (g_gamestate->player->last_direction == 2) {
				g_gamestate->projectiles[0]->dx = -1;
				g_gamestate->projectiles[0]->dy = 0;
			}
			if (g_gamestate->player->last_direction == 3) {
				g_gamestate->projectiles[0]->dx = 1;
				g_gamestate->projectiles[0]->dy = 0;
			}

	} else if (key == keybinds[5] || key == toupper(keybinds[5])) {
		displayMenu(MENU_PAUSE);

	} else if (key == keybinds[6]) 
		displayMenu(MENU_CONSOLE);

	


		// Seppuku
	if(key == 'q')
		EXIT = TRUE;

	if(key == '5')
		displayMenu(MENU_MAIN_MENU);

	if(key == 'g') {
	    if(g_gamestate->player->class == Priest) {
             g_gamestate->player->entity->health += imin((g_gamestate->player->entity->health)/4, g_gamestate->player->entity->maxHealth - g_gamestate->player->entity->health);
		 	}
	}

	if(key == 'h') {
	    //  if (g_gamestate->player->entity->coords->x == g_gamestate->chests->coords->x && g_gamestate->player->entity->coords->y == g_gamestate->chests->coords->y ) {
		    g_gamestate->player->item = get_random_item();
		//  }
		    if(strcmp(g_gamestate->player->item->id, "0018") == 0) {
			   g_gamestate->player->entity->maxHealth = (g_gamestate->player->entity->maxHealth)/1.5;
			   g_gamestate->player->entity->basedamage = (g_gamestate->player->entity->basedamage)*1.5;
			}
			if(strcmp(g_gamestate->player->item->id, "0019") == 0) {
			   g_gamestate->player->entity->basedamage = (g_gamestate->player->entity->basedamage)/1.5;
			   g_gamestate->player->entity->maxHealth = (g_gamestate->player->entity->maxHealth)*1.5;
			}
			if (g_gamestate->player->entity->maxHealth < g_gamestate->player->entity->health) {
				g_gamestate->player->entity->health = g_gamestate->player->entity->maxHealth;
			}
	}

}

void menu_keybinds(int key) {
	if (!isInMenu()) return;

	switch (key) {
		case KEY_B2:
		case '5': {
			Menu active_menu = getActiveMenu();
			if (active_menu == NULL) break;
			if (active_menu->id == MENU_MAIN_MENU) {
				closeMenu(MENU_MAIN_MENU);
				break;
			}
		}
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

// void move_projectile(int dx, int dy) {

// 	if(is_passable(g_gamestate->projectile->entity->coords->x + dx, g_gamestate->projectile->entity->coords->y + dy)) {
// 		g_gamestate->projectile->entity->coords->x += dx;
// 		g_gamestate->projectile->entity->coords->y += dy;
// 	}
// 	else {
// 		g_gamestate->projectile->entity->coords->x = g_gamestate->projectile->entity->coords->y = 0;
// 	}

// }

void move_projectile(int dx, int dy) {
	static int moveCount = 0;

	if (g_gamestate->player->class == Priest) {
	    g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->player->entity->coords->x + dx;
	    g_gamestate->projectiles[0]->entity->coords->y = g_gamestate->player->entity->coords->y + dy;

	} else if (g_gamestate->player->class == Mercenary) {
		if (moveCount < 5) {
			if (is_passable(g_gamestate->projectiles[0]->entity->coords->x + dx, g_gamestate->projectiles[0]->entity->coords->y + dy)) {
				g_gamestate->projectiles[0]->entity->coords->x += dx;
				g_gamestate->projectiles[0]->entity->coords->y += dy;
				moveCount++;
				if (moveCount > 10) {
					g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
				    moveCount = 0;
				}
			} else {
				g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
				moveCount = 0;
			}
		} else {
			if (is_passable(g_gamestate->projectiles[0]->entity->coords->x - dx, g_gamestate->projectiles[0]->entity->coords->y - dy)) {
				g_gamestate->projectiles[0]->entity->coords->x -= dx;
				g_gamestate->projectiles[0]->entity->coords->y -= dy;
				moveCount++;
				if (moveCount > 10) {
					g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
				    moveCount = 0;
				}
			} else {
				g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
				moveCount = 0;
			}
		}
	} else if (g_gamestate->player->class == Detective) {
		if (is_passable(g_gamestate->projectiles[0]->entity->coords->x + dx, g_gamestate->projectiles[0]->entity->coords->y + dy)) {
			g_gamestate->projectiles[0]->entity->coords->x += dx;
			g_gamestate->projectiles[0]->entity->coords->y += dy;
			moveCount++;
			if (moveCount > 10) {
					g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
				    moveCount = 0;
				}
		} else {
			g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
		}
	}
}

int is_passable(int x, int y){

    if (map[y][x] == 1){
        return 0;
    }

	if (map[y][x] == 5){	// mob ou chest
		return 0;
	}
	
    return 1;
}


void print_loading_screen(WINDOW* win, int HEIGHT, int WIDTH){

	for (int y = 0; y < HEIGHT; y++){
		
		for (int x = 0; x < WIDTH; x++){
			
			mvwaddch(win, y, x, ' ' | COLOR_BLACK);
		}
	}
}