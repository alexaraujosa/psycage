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
		114,		// 'r' | Use Projectile
		107,		// 'k' | Use Trap
		106,		// 'j' | Use Molotov
		108,		// 'l' | Use Ultimate
		112,		// 'p' | Open Pause Menu
		92			// '\\'| Open Console
};

int ALTURA_JOGO;
int LARGURA_JOGO;

Gamestate g_gamestate;
int** map_footprint;
int find_map;

// #define TICK_DURATION_MS 100
int TICK_DURATION_MS = 100;
int TICKS_PER_SECOND;
clock_t tickStart, tickEnd;
float tickDuration, taskDuration;

Gamestate init_gameloop() {
	TICKS_PER_SECOND = (1000 / TICK_DURATION_MS);

	debug_file(dbgOut, 0, " - Generating dimensions...\n");
	ALTURA_JOGO = g_renderstate->nrows - ALTURA_LOGO - 2;
	LARGURA_JOGO = g_renderstate->ncols - 2;

    debug_file(dbgOut, 0, " - Allocating gameloop memory...\n");

	Gamestate gs = (Gamestate)malloc(sizeof(GAMESTATE));

    debug_file(dbgOut, 0, " - Initializing player...\n");
	Player player = defaultPlayer();
	gs->player = player;

    debug_file(dbgOut, 0, " - Initializing projectiles...\n");
    int projectile_count = 3;
    Projectile* projectiles = (Projectile*)malloc(sizeof(Projectile) * projectile_count);
    for(int i = 0 ; i < projectile_count ; i++) {
            Projectile projectile = defaultProjectile();
            projectiles[i] = projectile;
    }
    gs->projectiles = projectiles;
    gs->projectile_count = projectile_count;

    debug_file(dbgOut, 0, " - Initializing clock-related variables...\n");
	gs->input_initialized = FALSE;
	gs->clock = 0;
	gs->block_clock = 1;
	gs->potion_strength = 0;

	gs->clocks = init_clocks();

	// gs->pointA = defaultCoords();
	// gs->pointB = defaultCoords();
	// gs->recalculate = 0;
	// gs->path_cell_count = 0;
	// gs->path_cells = NULL;







    debug_file(dbgOut, 0, " -- Generating light map...\n");
	init_light_map(ALTURA_JOGO, LARGURA_JOGO);

    // debug_file(dbgOut, 0, " - Generating initial map...\n");
	// find_map = create_random_map(ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X); // map

    debug_file(dbgOut, 0, " - Generating map footprint...\n");
	map_footprint = (int **)malloc((ALTURA_JOGO) * sizeof(int *));
    for (int i = 0; i < ALTURA_JOGO; i++) {
        map_footprint[i] = (int *)malloc((LARGURA_JOGO) * sizeof(int));
    }

	// for (int i = 0; i < ALTURA_JOGO; i++) {
	// 	for (int j = 0; j < (LARGURA_JOGO); j++) {
	// 		map_footprint[i][j] = map[i][j];
	// 	}
	// }

    // debug_file(dbgOut, 0, " - Generating mobs...\n");
	// int mob_count = 3;
	// Mob* mobs = (Mob*)malloc(sizeof(Mob) * mob_count);

	// for (int i = 0; i < mob_count; i++) {
	// 	Mob mob = defaultMob();
	// 	addRandomItemToMob(mob);

	// 	addMobToMap(mob, map, LARGURA_JOGO, ALTURA_JOGO);

	// 	mobs[i] = mob;
	// 	map[mob->entity->coords->y][mob->entity->coords->x] = 5;
	// }

	// gs->mobs = mobs;
	// gs->mob_count = mob_count;

	// debug_file(dbgOut, 0 , " - Generating chests...\n");
    // int chest_count = 1;
	// Chest* chests = (Chest*)malloc(sizeof(Chest) * chest_count);

	// for (int i = 0; i < chest_count; i++) {
	//     Chest chest = defaultChest();

	//     addChestToMap(chest, map, LARGURA_JOGO, ALTURA_JOGO);

	//     chests[i] = chest;
	//     map[chest->entity->coords->y][chest->entity->coords->x] = 5;
	// }

	// gs->chests = chests;
	// gs->chest_count = chest_count;

	find_map = -1;
	map = NULL;
	// map_footprint = NULL;

	// gs->mob_count = 0;
	// gs->mobs = NULL;

	// gs->chests = NULL;
	// gs->chest_count = 0;

	debug_file(dbgOut, 0, " - Generating mobs...\n");
	int mob_count = 3;
	Mob* mobs = (Mob*)malloc(sizeof(Mob) * mob_count);

	for (int i = 0; i < mob_count; i++) {
		Mob mob = defaultMob();
		mobs[i] = mob;

	}

	gs->mobs = mobs;
	gs->mob_count = mob_count;
	gs->mob_begin = mob_count;

	debug_file(dbgOut, 0 , " - Generating chests...\n");
    int chest_count = 1;
	Chest* chests = (Chest*)malloc(sizeof(Chest) * chest_count);

	for (int i = 0; i < chest_count; i++) {
	    Chest chest = defaultChest();

	    chests[i] = chest;
	    // map[chest->entity->coords->y][chest->entity->coords->x] = 5;
	}

	gs->chests = chests;
	gs->chest_count = chest_count;

	gs->last_res = -1;

	gs->paused = FALSE;
	gs->valid_state = TRUE;

	g_gamestate = gs;

	init_potions_clock();
	init_grenades_clock();
	init_ultimate_clocks();

	return gs;
}

void tick() {
	tickStart = clock();

	// ListNode clk = g_gamestate->clocks;
	// while (clk != NULL) {
	// 	Clock node_clock = (Clock)clk->data;
	// 	if (!node_clock->blocked) {
	// 		tickClock(node_clock);
	// 	}

	// 	clk = clk->next;
	// }
	LinkedList ll = g_gamestate->clocks;
	if (ll == NULL) return;

    ListNode current = ll->head;
    while (current != NULL) {
        Clock data = (Clock)current->data;

        if (!data->blocked) {
			tickClock(data);
		}
        
        current = current->next;
    }

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

		if (is_player_insane(g_gamestate->player)) {
			static int iClockInitialized = FALSE;
			static Clock iClock;

			if (!iClockInitialized) {
				debug_file(dbgOut, 2, "Initalized insanity clock.\n");

				iClock = defaultClock();
				iClock->blocked = FALSE;
				iClock->maxTicks = TICKS_PER_SECOND;

				addClock(iClock);

				iClockInitialized = TRUE;
			}

			if (iClock->ticks == TICKS_PER_SECOND - 1) {
				debug_file(dbgOut, 2, "Attempted to reduce one sanity.\n");

				reduce_sanity(g_gamestate->player, 1);
				if (g_gamestate->player->sanity == -100) {
					debug_file(dbgOut, 2, "Minimum insanity. Time to die.\n");
					g_gamestate->player->entity->health = 0;
				}
			}
		}

		// if (get_candle_light(g_gamestate->player) > 0) {
		{
			static int cClockInitialized = FALSE;
			static Clock cClock;

			if (!cClockInitialized) {
				debug_file(dbgOut, 2, "Initalized candle light clock.\n");

				cClock = defaultClock();
				cClock->blocked = TRUE;
				cClock->maxTicks = 10 * TICKS_PER_SECOND;

				addClock(cClock);

				cClockInitialized = TRUE;
			} else {
				cClock->blocked = FALSE;
			}

			if (g_gamestate->player->current_candle > 0) {
				if (cClock->ticks == (5 * TICKS_PER_SECOND) - 1) {
					debug_file(dbgOut, 2, "Attempted to reduce one candle fuel.\n");

					reduce_from_current_candle(g_gamestate->player, 1);
					if (g_gamestate->player->current_candle == 0) {
						debug_file(dbgOut, 2, "No candle fuel. Starting to reduce sanity\n");
						// cClock->blocked = TRUE;
					}
				}
			} else {
				if (cClock->ticks == (10 * TICKS_PER_SECOND) - 1) {
					debug_file(dbgOut, 2, "Attempted to reduce one sanity.\n");

					reduce_sanity(g_gamestate->player, 10);
				}
			}
		}

		molotov_mob_checker();
		molotov_player_checker();

		for (int i = 0; i < g_gamestate->mob_begin; i++) {
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

		if(g_gamestate->projectiles[1]->entity->coords->x != 0 && g_gamestate->projectiles[1]->entity->coords->y != 0) {
            move_trap(g_gamestate->projectiles[1]->dx, g_gamestate->projectiles[1]->dy); 	
			trap_checker();
		}

		if(g_gamestate->projectiles[2]->entity->coords->x != 0 && g_gamestate->projectiles[2]->entity->coords->y != 0) {
            move_molotov(g_gamestate->projectiles[2]->dx, g_gamestate->projectiles[2]->dy);
            molotov_checker();        
        }

		if(map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] == 7)
			use_random_potion();

		if(map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] == 6) {
			// int candle_val = (rand() % 3) + 1;
			// int sanity = (rand() % 20) + 5;

			// add_candle_fuel(g_gamestate->player, candle_val);
			// restore_sanity(g_gamestate->player, sanity);

			int candle_val = (rand() % 3) + 1;
			add_candle_fuel(g_gamestate->player, candle_val);

			map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] = 
				map_footprint[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x];
		}

		// RTX_ON
		calculate_visibility(
			g_gamestate->player->entity->coords->x, 
			g_gamestate->player->entity->coords->y, 
			map, 
			ALTURA_JOGO, 
			LARGURA_JOGO
		); 
		
		ultimate_checker();

	for(int i = 0 ; i < g_gamestate->mob_begin ; i++) {
		MobAliveChecker(g_gamestate->mobs[i]);	
	}
	
		// Doors
		if(g_gamestate->mob_count <= 0){

			static int d = 1;
			if(d > 0){
				doors(
					g_gamestate->player->entity->coords->x, 
					g_gamestate->player->entity->coords->y, 
					g_gamestate->player->radius, 
					ALTURA_JOGO, 
					LARGURA_JOGO
				);
				d--;
			}

			if(map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] == 4){
				d = 1;
				continue_game();
			}
		}
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


	godmode_code_checker(key);
	vision_code_checker(key);

	if(key == 'k')
		g_gamestate->player->kills++;

	
	
	if(key == 'j')
		g_gamestate->player->hasUltimate = 1;




	if(key == 'z')
		g_gamestate->mob_count = 0; //continue_game();

    // Movement Controls
    


	if(key == keybinds[0] || key == toupper(keybinds[0])) {
		if(is_passable(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y-1)){
			move_player(0, -1);
			g_gamestate->player->last_direction = 0; 
		}
	} else if (key == keybinds[1] || key == toupper(keybinds[1])) {	
		if(is_passable(g_gamestate->player->entity->coords->x, g_gamestate->player->entity->coords->y+1)){
			move_player(0, 1);
			g_gamestate->player->last_direction = 1; 
		}
	} else if (key == keybinds[2] || key == toupper(keybinds[2])) {
			if(is_passable(g_gamestate->player->entity->coords->x-1, g_gamestate->player->entity->coords->y)){
				move_player(-1, 0);
				g_gamestate->player->last_direction = 2;
			}  
	} else if (key == keybinds[3] || key == toupper(keybinds[3])) {
			if(is_passable(g_gamestate->player->entity->coords->x+1, g_gamestate->player->entity->coords->y)){
				move_player(1, 0);
				g_gamestate->player->last_direction = 3;
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
	} else if((key == keybinds[5] || key == toupper(keybinds[5])) && g_gamestate->player->trap != 0) {
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
    } else if((key == keybinds[6] || key == toupper(keybinds[6])) && g_gamestate->player->molotov != 0) {
            if(g_gamestate->projectiles[2]->entity->coords->x == 0 && g_gamestate->projectiles[2]->entity->coords->y == 0){
                g_gamestate->projectiles[2]->entity->coords->x = g_gamestate->player->entity->coords->x;
                g_gamestate->projectiles[2]->entity->coords->y = g_gamestate->player->entity->coords->y;
            }
            if (g_gamestate->player->last_direction == 0) {
                    g_gamestate->projectiles[2]->dx = 0;
                    g_gamestate->projectiles[2]->dy = -1;
            }
            if (g_gamestate->player->last_direction == 1) {
                    g_gamestate->projectiles[2]->dx = 0;
                    g_gamestate->projectiles[2]->dy = 1;
            }
            if (g_gamestate->player->last_direction == 2) {
                    g_gamestate->projectiles[2]->dx = -1;
                    g_gamestate->projectiles[2]->dy = 0;
            }
               if (g_gamestate->player->last_direction == 3) {
                    g_gamestate->projectiles[2]->dx = 1;
                    g_gamestate->projectiles[2]->dy = 0;
            }
    } else if (key == keybinds[7] || key == toupper(keybinds[7])) {
		if(g_gamestate->player->hasUltimate)
			ultimate_use(); 
	} else if (key == keybinds[8] || key == toupper(keybinds[8])) {
		displayMenu(MENU_PAUSE);
	} else if (key == keybinds[9]) 
		displayMenu(MENU_CONSOLE);

	


	// Seppuku
	if(key == 'q')
		EXIT = TRUE;

	if(key == '5')
		displayMenu(MENU_MAIN_MENU);

	if(key == 'h') {
		if((g_gamestate->chests[0]->entity->coords->y == g_gamestate->player->entity->coords->y+1 && 
               g_gamestate->chests[0]->entity->coords->x == g_gamestate->player->entity->coords->x) ||
               (g_gamestate->chests[0]->entity->coords->y == g_gamestate->player->entity->coords->y-1 && 
               g_gamestate->chests[0]->entity->coords->x == g_gamestate->player->entity->coords->x) ||
               (g_gamestate->chests[0]->entity->coords->y == g_gamestate->player->entity->coords->y && 
               g_gamestate->chests[0]->entity->coords->x == g_gamestate->player->entity->coords->x+1) ||
               (g_gamestate->chests[0]->entity->coords->y == g_gamestate->player->entity->coords->y && 
               g_gamestate->chests[0]->entity->coords->x == g_gamestate->player->entity->coords->x-1)
            ) {
		    map[g_gamestate->chests[0]->entity->coords->y][g_gamestate->chests[0]->entity->coords->x] = map_footprint[g_gamestate->chests[0]->entity->coords->y][g_gamestate->chests[0]->entity->coords->x];
            g_gamestate->chests[0]->entity->coords->y = 0;
            g_gamestate->chests[0]->entity->coords->x = 6; 
		    g_gamestate->player->item = get_random_item();
		    if(strcmp(g_gamestate->player->item->id, "0018") == 0) {
			   g_gamestate->player->entity->maxHealth = (g_gamestate->player->entity->maxHealth)/1.5;
			   g_gamestate->player->entity->basedamage = (g_gamestate->player->entity->basedamage)*1.5;
			}
			if (g_gamestate->player->entity->maxHealth < g_gamestate->player->entity->health) {
				g_gamestate->player->entity->health = g_gamestate->player->entity->maxHealth;
			}
        	g_gamestate->player->entity->armor = g_gamestate->player->item->armor;
		}
	}
	if (key == 'c') {
		// if (is_player_insane(g_gamestate->player)) g_gamestate->player->sanity = 99;
		// else g_gamestate->player->sanity = 0;
		use_candle_fuel(g_gamestate->player, 1);
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

			break;
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

void move_projectile(int dx, int dy) {
	static int moveCount = 0;

	if (g_gamestate->player->class == Priest) {
		if (is_passable(g_gamestate->projectiles[0]->entity->coords->x + dx, g_gamestate->projectiles[0]->entity->coords->y + dy)) {
			g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->player->entity->coords->x + dx;
	        g_gamestate->projectiles[0]->entity->coords->y = g_gamestate->player->entity->coords->y + dy;
		} else {
			g_gamestate->projectiles[0]->entity->coords->x = g_gamestate->projectiles[0]->entity->coords->y = 0;
		}
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

void destroy_mob(int x) {
  if (x >= g_gamestate->mob_count) return;

  for (int i = 0; i < g_gamestate->mob_count - 1; i++) {
    if (i >= x) {
      destroyMob(g_gamestate->mobs[i]);
      g_gamestate->mobs[i] = g_gamestate->mobs[i + 1];
    }
  }

  g_gamestate->mob_count--;
}
